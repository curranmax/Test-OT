
#include <cryptoTools/Common/Defines.h>

#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/IOService.h>

#include "libOTe/Base/BaseOT.h"

#include <array>

using namespace osuCrypto;

#define K 4

int main(int argc, char const *argv[]) {
	// This is the desired index to fetch from the other party.
	int desired_index = 0;

	// Goal is to get the value of sender.table[desired_index] into fetch_entry
	std::vector<int> fetched_entry; // Can assume the size will be K

	// ------------------------------------
	// | Initialize communication objects |
	// ------------------------------------
	// Initializes random number generator.
	PRNG prng(_mm_set_epi32(1, 2, 3, 4));

	// Initializes IOService
	int num_io_threads = 4;
	IOService ios(num_io_threads);

	// Const values used to connect receiver and sender
	const std::string server_addr = "127.0.0.1:1212";
	const std::string connection_name = "party0_party1";
	const std::string channel_name = "channel";

	// Creates the endpoint. (May need to sway EpModes).
	Endpoint ep(ios, server_addr, EpMode::Client, connection_name);

	// Creates the channel used to send data between the two.
	Channel ch = ep.addChannel(channel_name);

	ch.waitForConnection();

	// OT

	return 0;
}