//
// Created by poilk on 2022/7/9.
//

#ifndef PENDUO_SRC_NET_TCPCONNECTION_H_
#define PENDUO_SRC_NET_TCPCONNECTION_H_

#include <memory>
#include <string>
#include "base/noncopyable.h"
#include "net/Callbacks.h"
#include "net/InetAddress.h"
#include "net/Buffer.h"

namespace penduo {

class EventLoop;
class Socket;
class Channel;

class TcpConnection : noncopyable,
                      public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(EventLoop *loop, std::string name, int socket_fd, InetAddress local_addr, InetAddress peer_addr);
  bool connected() const {return state_ == Connected;}
  std::string name() const {return name_;}
  InetAddress peer_address() const {return peer_addr_;}
  InetAddress local_address() const {return local_addr_;}
  void set_connection_callback(ConnectionCallback cb) {connection_callback_ = std::move(cb);}
  void set_close_callback(CloseCallback cb) {close_callback_ = std::move(cb);}
  void set_message_callback(MessageCallback cb) {message_callback_ = std::move(cb);}


  // called when TcpServer accepts a new connection
  void connect_establish();   //should be called only once
  // called when TcpServer has remove me from its map
  void connect_destoryed();   //should be called only once
 private:
  enum StateE { Connecting, Connected, DisConnected};

  void set_state(StateE state){state_ = state;}
  void handle_read(Timestamp receive_time);
  void handle_write();
  void handle_close();
  void handle_error();

  EventLoop *loop_;
  const std::string name_;
  StateE state_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  InetAddress local_addr_;
  InetAddress peer_addr_;
  ConnectionCallback connection_callback_;
  CloseCallback close_callback_;
  MessageCallback message_callback_;
  Buffer input_buffer_;
};

} // penduo

#endif //PENDUO_SRC_NET_TCPCONNECTION_H_
