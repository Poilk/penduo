//
// Created by poilk on 2022/7/9.
//

#include <unistd.h>
#include "base/logging/Logger.h"
#include "net/InetAddress.h"
#include "net/EventLoop.h"
#include "net/TcpServer.h"
#include "net/TcpConnection.h"

using namespace penduo;

void on_connection(const TcpConnectionPtr &connection) {
  if (connection->connected()) {
    LOG_INFO << "on_connection: new connection [" << connection->name() << "] from "
             << connection->peer_address().to_ip_port();
  } else{
    LOG_INFO << "on_connection(): connection [" << connection->name() << "] is down";
  }
}

void on_message(const TcpConnectionPtr &connection, const char *data, size_t len){
  LOG_INFO << "on_message(): received " << len << " bytes from connection [" << connection->name()
  << "]";
  LOG_INFO << "on_message(): reecived message: " << data;
}

int main(int argc, char *argv[]) {
  LOG_INFO << "main(): pid = " << getpid();

  InetAddress listen_addr(9981);
  EventLoop loop;
  TcpServer server(&loop, listen_addr, "tcpserver_v1");
  server.set_connection_callback(on_connection);
  server.set_message_callback(on_message);
  server.start();

  loop.loop();
  return 0;
}