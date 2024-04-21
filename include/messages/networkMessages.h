#ifndef _ENGINE_LOGICCONTROLLER_H_
#define _ENGINE_LOGICCONTROLLER_H_

#include <networkingHelper/networkMessage.hpp>
#include <string>
#include <vector>


namespace SFG {
namespace Messages {

class MsgContent {
  public:
  std::string username = "";
  std::string msgText = "";

  bool operator==( MsgContent const& b ) const;

  private:
  friend class bitsery::Access;
  template < typename S >
  void serialize( S& s ) {
    s.text1b( username, 1024 );
    s.text1b( msgText, 1024 );
  }
  template < typename S >
  void deserialize( S& s ) {
    s.text1b( username, 1024 );
    s.text1b( msgText, 1024 );
  }
};

class SendMessageRequest {
  public:
  MsgContent msg = {};

  bool operator==( SendMessageRequest const& b ) const;

  private:
  friend class bitsery::Access;
  template < typename S >
  void serialize( S& s ) {
    s.object( msg );
  }
  template < typename S >
  void deserialize( S& s ) {
    s.object( msg );
  }
};

class SendMessageResponse {
  public:
  bool success = false;
  std::string errorMsg = "";

  bool operator==( SendMessageResponse const& b ) const;

  private:
  friend class bitsery::Access;
  template < typename S >
  void serialize( S& s ) {
    s.boolValue( success );
    s.text1b( errorMsg, 1024 );
  }
  template < typename S >
  void deserialize( S& s ) {
    s.boolValue( success );
    s.text1b( errorMsg, 1024 );
  }
};

class GetAllMessagesRequest {
  public:
  bool operator==( GetAllMessagesRequest const& b ) const;

  private:
  friend class bitsery::Access;
  template < typename S >
  void serialize( S& s ) {}
  template < typename S >
  void deserialize( S& s ) {}
};

class AllMessages {
  public:
  std::vector< MsgContent > msgList = {};

  bool operator==( AllMessages const& b ) const;

  private:
  friend class bitsery::Access;
  template < typename S >
  void serialize( S& s ) {
    s.container( msgList, 1024 );
  }
  template < typename S >
  void deserialize( S& s ) {
    s.container( msgList, 1024 );
  }
};

class ReqRepErrorMessage {
  public:
  bool operator==( ReqRepErrorMessage const& b ) const;

  private:
  friend class bitsery::Access;
  template < typename S >
  void serialize( S& s ) {}
  template < typename S >
  void deserialize( S& s ) {}
};

}  // namespace Messages
}  // namespace SFG

#endif /* _ENGINE_LOGICCONTROLLER_H_ */
