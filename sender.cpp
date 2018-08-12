
#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/IOService.h>

#include "libOTe/Base/BaseOT.h"

#include <array>
#include <vector>

// Constants related to table
#define N 10
#define K 4

int main(int argc, char const *argv[]) {
	// This is the table that will be read from.
	std::vector<std::array<int, K> > table;

	// Adds some data to the table
	for(int i = 0; i < N; ++i) {
		std::array<int, K> v;
		v.fill(i);
		table.append(v);
	}

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
	Endpoint ep(ios, server_addr, EpMode::Server, connection_name);

	// Creates the channel used to send data between the two.
	Channel ch = ep.addChannel(channel_name);

	return 0;
}