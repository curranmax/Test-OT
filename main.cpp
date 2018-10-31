
#include <cryptoTools/Common/Defines.h>

#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/IOService.h>

#include "libOTe/Base/BaseOT.h"
#include "libOTe/NChooseOne/Oos/OosNcoOtSender.h"
#include "libOTe/NChooseOne/Oos/OosNcoOtReceiver.h"

#include <array>

using namespace osuCrypto;

// N is the number of entries in the table, and K is the number of 64-bit integers in each entry

// Grid Table:
//   N -> 10,000
//   K -> 22

// PU Table:
//   N -> 400
//   K -> 18

#define N 400
#define K 18

int main(int argc, char const *argv[]) {
	PRNG prng0(_mm_set_epi32(1, 2, 3, 4)), prng1(_mm_set_epi32(1, 2, 3, 5));

	// Set up network
	int num_io_threads = 4;
	IOService ios(num_io_threads);

	// Const values used to connect receiver and sender
	const std::string server_addr = "127.0.0.1:1212";
	const std::string connection_name = "party0_party1";
	const std::string channel_name = "channel";

	std::cout << "Creating Channels" << std::endl;

	// Creates the endpoint. (May need to sway EpModes).
	Endpoint ep_server(ios, server_addr, EpMode::Server, connection_name);
	Endpoint ep_client(ios, server_addr, EpMode::Client, connection_name);

	// Creates the channel used to send data between the two.
	Channel ch_server = ep_server.addChannel(channel_name);
	Channel ch_client = ep_client.addChannel(channel_name);

	ch_server.waitForConnection();
	ch_client.waitForConnection();

	std::cout << "Channels connected" << std::endl;

	// Set up the OT sender and receiver
	const u64 step = ceil(K * 64.0 / 512.0);
	u64 num_ots = step * N;

	std::cout << "Step: " << step << std::endl << "Num OTs: " << num_ots << std::endl;
	
	OosNcoOtSender sender;
	OosNcoOtReceiver recver;

	bool malicious_secure = false;
	int sec_param = 40, input_bit_count = 50;
	sender.configure(malicious_secure, sec_param, input_bit_count);
	recver.configure(malicious_secure, sec_param, input_bit_count);

	u64 bc_recv = recver.getBaseOTCount(), bc_send = sender.getBaseOTCount();

	if(bc_recv != bc_send) {
		std::cerr << "Mismatch base OT count: " << bc_recv << ", " << bc_send << std::endl;
		exit(1);
	}
	u64 base_count = bc_recv;

	std::vector<block> base_recv(base_count);
	std::vector<std::array<block, 2>> base_send(base_count);
	BitVector base_choice(base_count);
	base_choice.randomize(prng0);

	prng0.get((u8*)base_send.data()->data(), sizeof(block) * 2 * base_send.size());
	for(u64 i = 0; i < base_count; ++i) {
		base_recv[i] = base_send[i][base_choice[i]];
	}

	sender.setBaseOts(base_recv, base_choice);
	recver.setBaseOts(base_send);

	// Sender and recver routines
	auto send_routine = [&]() {
		std::cout << "Running sendRoutine" << std::endl;
		sender.init(num_ots, prng0, ch_server);
		
		for(u64 i = 0, n = 0; i < num_ots; ++n) {
			auto min = std::min<u64>(num_ots - i, step);

			sender.recvCorrection(ch_server, min);

			// Now we encode any OT message with index less that i + min.
			if(n == 68) {
				for(u64 j = 0; j < min; ++j, ++i) {
					auto choice = toBlock(j, i);

					block otMessage;

					sender.encode(i, &choice, &otMessage);

					std::cout << "Sender:   " << choice << ", " << otMessage << std::endl;
				}
			} else {
				i += min;
			}
		}
	};

	auto recv_routine = [&]() {
		std::cout << "Running recv_routine" << std::endl;

		recver.init(num_ots, prng1, ch_client);

		for(u64 i = 0, n = 0; i < num_ots; ++n) {
			auto min = std::min<u64>(num_ots - i, step);

			bool print = (n == 68);
			for(u64 j = 0; j < min; ++j, ++i) {
				block choice = toBlock(j, i) /*prng1.get<block>()*/;

				block otMessage = toBlock(j, i);

				recver.encode(i, &choice, &otMessage);

				//message received
				if(print) {
					std::cout << "Receiver: " << choice << ", " << otMessage << std::endl;
				}
			}

			// After encoding, we have the OTMessage. We can send info over 
			// message to send next min correction values.
			recver.sendCorrection(ch_client, min);
		}

		// recver.check(ch, ZeroBlock);
	};

	auto recv_thread = std::thread(recv_routine);
	auto send_thread = std::thread(send_routine);

	recv_thread.join();
	send_thread.join();

	return 0;
}
