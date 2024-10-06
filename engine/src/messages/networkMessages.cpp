#include "messages/networkMessages.h"


bool SFG::Messages::MsgContent::operator==( SFG::Messages::MsgContent const& b ) const {
  bool ret = true;
  ret = ret && ( this->username == b.username );
  ret = ret && ( this->msgText == b.msgText );
  return ret;
}

bool SFG::Messages::SendMessageRequest::operator==( SFG::Messages::SendMessageRequest const& b ) const {
  bool ret = true;
  ret = ret && ( this->msg == b.msg );
  return ret;
}

bool SFG::Messages::SendMessageResponse::operator==( SFG::Messages::SendMessageResponse const& b ) const {
  bool ret = true;
  ret = ret && ( this->success == b.success );
  ret = ret && ( this->errorMsg == b.errorMsg );
  return ret;
}

bool SFG::Messages::GetAllMessagesRequest::operator==( SFG::Messages::GetAllMessagesRequest const& b ) const {
  bool ret = true;
  return ret;
}

bool SFG::Messages::AllMessages::operator==( SFG::Messages::AllMessages const& b ) const {
  bool ret = true;
  ret = ret && ( this->msgList == b.msgList );
  return ret;
}

bool SFG::Messages::ReqRepErrorMessage::operator==( SFG::Messages::ReqRepErrorMessage const& b ) const {
  bool ret = true;
  return ret;
}
