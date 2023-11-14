#pragma once

// library headers
#include <zmq.hpp>

namespace SFG {
static zmq::context_t global_inproc_context_ = zmq::context_t( 1 );
}
