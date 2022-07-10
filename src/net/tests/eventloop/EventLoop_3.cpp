//
// Created by poilk on 2022/6/28.
//

#include <chrono>
#include <iostream>

#include "base/timerfd/TimerfdCreator.h"
#include "net/EventLoop.h"
#include "net/Channel.h"
#include "base/logging/Logger.h"

penduo::EventLoop *g_loop;
penduo::Timerfd *timerfd;
using namespace penduo;

void func_in_loop() {
  LOG_INFO << "func_in_loop!" << std::endl;
  g_loop->assert_in_loop_thread();
}

void timeout(Timestamp receive_time) {
  LOG_INFO << "Timeout !!!" << std::endl;
  auto t1 = std::chrono::system_clock::now();
  auto t2 = std::chrono::system_clock::now();
  auto t3 = std::chrono::steady_clock::now();
  auto t4 = std::chrono::steady_clock::now();
  std::cout << (t2 - t1).count() << ' ' << (t4 - t3).count() << std::endl;
  std::cout << (t2 - t1).count() << ' ' << (t4 - t3).count() << std::endl;
  std::cout << "poll to timeout differ "
            << Timestamp::differ<Timestamp::TsMicroseconds>(receive_time, Timestamp::now()) << std::endl;
  auto t = (t2 - t1).count();

  timerfd->read();
  auto expiration = Timestamp::add<Timestamp::TsMilliseconds>(Timestamp::now(), 2000);
  //timerfd->reset(expiration);
  g_loop->run_in_loop(func_in_loop);
  //g_loop->quit();
}

int main(int argc, char *argv[]) {
  penduo::EventLoop loop;
  g_loop = &loop;
  timerfd = penduo::TimerfdCreator::new_default();
  penduo::Channel channel(&loop, timerfd->get_fd());
  channel.set_read_callback(timeout);
  channel.enable_reading();

  int64_t delay_ms = 2000;
  LOG_INFO << "before reset timerfd, delay_ms: " << delay_ms;
  auto expiration = Timestamp::add<Timestamp::TsMilliseconds>(Timestamp::now(), delay_ms);
  timerfd->reset(expiration);

  //g_loop->run_in_loop(func_in_loop);

  loop.loop();

  return 0;
}