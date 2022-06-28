//
// Created by poilk on 2022/6/28.
//

#include <sys/timerfd.h>

#include <iostream>

#include "net/EventLoop.h"
#include "net/Channel.h"
#include "base/logging/Logger.h"

penduo::EventLoop *g_loop;
using namespace penduo;

void timeout(){
  LOG_INFO << "Timeout !!!" << std::endl;
  g_loop->quit();
}

int main(int argc, char *argv[]){
  penduo::EventLoop loop;
  g_loop = &loop;

  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  penduo::Channel channel(&loop, timerfd);
  channel.set_read_callback(timeout);
  channel.enable_reading();

  struct itimerspec how_long{};
  how_long.it_value.tv_sec = 5;
  ::timerfd_settime(timerfd, 0, &how_long, nullptr);

  loop.loop();

  return 0;
}