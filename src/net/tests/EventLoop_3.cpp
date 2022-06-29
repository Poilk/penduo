//
// Created by poilk on 2022/6/28.
//

#include <sys/timerfd.h>
#include <chrono>
#include <iostream>

#include "net/EventLoop.h"
#include "net/Channel.h"
#include "base/logging/Logger.h"

penduo::EventLoop *g_loop;
int timerfd;
using namespace penduo;

void timeout(){
  LOG_INFO << "Timeout !!!" << std::endl;
  auto t1 = std::chrono::system_clock::now();
  auto t2 = std::chrono::system_clock::now();
  auto t3 = std::chrono::steady_clock::now();
  auto t4 = std::chrono::steady_clock::now();
  std::cout << (t2 - t1).count() << ' ' << (t4 - t3).count() << std::endl;
  std::cout << (t2 - t1).count() << ' ' << (t4 - t3).count() << std::endl;
  auto t = (t2 - t1).count();

  read(timerfd, nullptr, 8);
  //g_loop->quit();
}

int main(int argc, char *argv[]){
  penduo::EventLoop loop;
  g_loop = &loop;

  timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  penduo::Channel channel(&loop, timerfd);
  channel.set_read_callback(timeout);
  channel.enable_reading();

  struct itimerspec how_long{};
  how_long.it_value.tv_sec = 2;
  how_long.it_interval.tv_sec = 2;
  ::timerfd_settime(timerfd, 0, &how_long, nullptr);

  loop.loop();

  return 0;
}