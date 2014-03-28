// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "rpc_service.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <string>
#include "proxy.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::rpc;

class rpc_serviceHandler : virtual public rpc_serviceIf {
 public:

  Proxy proxy;

  rpc_serviceHandler() {
    // Your initialization goes here
    common::CachePolicy policy = common::RANDOM;
    common::CacheSize cache_size = common::TWO_MB;
    proxy.init(policy, cache_size);
  }

  void fetch(std::string& _return, const std::string& s) {
    _return = proxy.fetch(s);
  }

  void restart_debug(const int32_t cache_policy, const int32_t cache_size) {
    proxy.restart_debug(cache_policy, cache_size);
  }

  void dump_stats(const std::string& batch_type) {
    proxy.dump_stats(batch_type);
  }

};

int main(int argc, char **argv) {
  int port = 9091;
  shared_ptr<rpc_serviceHandler> handler(new rpc_serviceHandler());
  shared_ptr<TProcessor> processor(new rpc_serviceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}
