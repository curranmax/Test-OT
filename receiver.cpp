
#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/IOService.h>

#include "libOTe/Base/BaseOT.h"

#include <array>

#define K 4

int main(int argc, char const *argv[]) {
	// This is the desired index to fetch from the other party.
	int desired_index = 0;

	// Goal is to get the value of sender.table[desired_index] into fetch_entry
	std::array<int, K> fetched_entry;

	// ------------------------------------
	// | Initialize communication objects |
	// ------------------------------------
	// Initializes random number generator.
	RNG prng(_mm_set_epi32(1, 2, 3, 4));

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

	return 0;
}