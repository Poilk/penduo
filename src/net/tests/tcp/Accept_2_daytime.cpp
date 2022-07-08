//
// Created by poilk on 2022/7/8.
//

#include <iostream>
#include <string>
#include <unistd.h>

#include "net/InetAddress.h"
#include "net/SocketsOps.h"
#include "net/EventLoop.h"
#include "net/Acceptor.h"
#include "base/Daytime.h"

using namespace penduo;

void daytime_connect(int socket_fd, InetAddress peer_addr){
  std::cout << " new_connect(): accepted a new connection from " << peer_addr.to_ip_port() << std::endl;
  std::string write_string = Daytime::get_current_formatted_time() + "\n";
  ::write(socket_fd, write_string.data(), write_string.size());
  sockets::close(socket_fd);
}

void hello_connect(int socket_fd, InetAddress peer_addr){
  std::cout << " new_connect(): accepted a new connection from " << peer_addr.to_ip_port() << std::endl;
  std::string write_string = "Hello penduo!\n";
  ::write(socket_fd, write_string.data(), write_string.size());
  sockets::close(socket_fd);
}

int main(int argc, char *argv[]){
  std::cout << "main(): pid = " << getpid() << std::endl;

  EventLoop loop;

  InetAddress day_time_listen_addr(9980);
  std::cout << "daytime listen_addr" << day_time_listen_addr.to_ip_port() << std::endl;

  Acceptor day_time_acceptor(&loop, day_time_listen_addr, true);
  day_time_acceptor.set_new_connection_callback(daytime_connect);
  day_time_acceptor.listen();


  InetAddress hello_listen_addr(9981);
  std::cout << "daytime listen_addr" << hello_listen_addr.to_ip_port() << std::endl;

  Acceptor hello_acceptor(&loop, hello_listen_addr, true);
  hello_acceptor.set_new_connection_callback(hello_connect);
  hello_acceptor.listen();


  loop.loop();

  return 0;
}