#include "rpc_service.h" 
#include "common.h"
#include "batch_test.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace rpc;
using namespace std;

int main(int argc, char **argv) {
	// Specify server here
	boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9091));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	// Open communication
	rpc_serviceClient rpc_handle(protocol);
	transport->open();

	// Run test harness
	batch_test(&rpc_handle);

	// Close communication
	transport->close();
	return 0;
}
