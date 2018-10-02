
#include <cryptoTools/Common/Defines.h>

#include <cryptoTools/Network/Channel.h>
#include <cryptoTools/Network/IOService.h>

#include "libOTe/Base/BaseOT.h"

#include <array>
#include <vector>

using namespace osuCrypto;

// Constants related to table
// Ideally N would be ~1,000,000, and K would be 150
#define N 10
#define K 4

int main(int argc, char const *argv[]) {
	// This is the table that will be read from.
	std::vector<std::vector<int> > table;

	// Adds some data to the table
	for(int i = 0; i < N; ++i) {
		table.push_back(std::vector<int>(K, i));
	}

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
	Endpoint ep(ios, server_addr, EpMode::Server, connection_name);

	// Creates the channel used to send data between the two.
	Channel ch = ep.addChannel(channel_name);

	ch.waitForConnection();

	// OT

	return 0;
}