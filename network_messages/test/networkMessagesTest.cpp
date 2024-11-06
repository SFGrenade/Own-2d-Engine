#include <SFG/Own2dEngine/Logger/loggerFactory.h>
#include <SFG/Own2dEngine/NetworkMessages/Messages.pb.h>
#include <gtest/gtest.h>
#include <zmqPb/pair.hpp>

namespace SOENM = SFG::Own2dEngine::NetworkMessages;

TEST( NetworkMessages, NetworkingTest ) {
  SFG::Own2dEngine::Logger::spdlogger logger = SFG::Own2dEngine::Logger::LoggerFactory::get_logger( "NetworkingTest" );
  zmq::context_t context( 1 );

  std::string endPoint = "inproc://NetworkMessages_NetworkingTest";

  ZmqPb::Pair pairPart1( endPoint, true, &context );
  zmq_sleep( 1 );
  ZmqPb::Pair pairPart2( endPoint, false, &context );
  zmq_sleep( 1 );

  bool gotMessage = false;

  SOENM::ReqRepErrorMessage* subScribedMsg = new SOENM::ReqRepErrorMessage();
  pairPart1.subscribe( subScribedMsg, [logger, &gotMessage]( google::protobuf::Message const& req ) {
    SOENM::ReqRepErrorMessage const& actualReq = static_cast< SOENM::ReqRepErrorMessage const& >( req );
    gotMessage = true;
  } );

  SOENM::ReqRepErrorMessage* msg = new SOENM::ReqRepErrorMessage();
  pairPart2.sendMessage( msg );

  bool waitOneMoreCycle = false;
  for( int _ = 0; ( _ < 10 ) && ( !waitOneMoreCycle ); _++ ) {
    waitOneMoreCycle = gotMessage;
    EXPECT_NO_THROW( pairPart1.run() );
    EXPECT_NO_THROW( pairPart2.run() );
    zmq_sleep( 1 );
  }

  EXPECT_TRUE( gotMessage );

  EXPECT_TRUE( true );
}

TEST( NetworkMessages, Example ) {
  EXPECT_TRUE( true );
}
