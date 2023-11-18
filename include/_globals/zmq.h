#pragma once

// library headers
#include <zmq.hpp>
#include <zmqPb/zmqWrap.hpp>

namespace SFG {
// static zmq::context_t global_inproc_context_ = zmq::context_t( 0 );

template < class T >
static void EmptySubscribe( ZmqPb::ZmqWrap& zmqPbNetwork ) {
  zmqPbNetwork.subscribe( new T(), []( google::protobuf::Message const& ) {} );
}
}  // namespace SFG
