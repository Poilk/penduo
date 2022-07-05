//
// Created by poilk on 2022/6/28.
//

#include <sys/timerfd.h>
#include <chrono>
#include <iostream>
#include <map>

#include "net/EventLoop.h"
#include "net/Channel.h"
#include "base/logging/Logger.h"

penduo::EventLoop *g_loop;
int timerfd;
using namespace penduo;

std::map<int64_t, Timestamp> last_mp;

Timestamp st;

void func_in_loop(int64_t interval_ms) {
  Timestamp now = Timestamp::now();
  LOG_INFO << "func_in_loop set: " << interval_ms << " interval differ: " <<
           Timestamp::differ<Timestamp::TsMicroseconds>(last_mp[interval_ms], now) <<
           "init differ: " << Timestamp::differ<Timestamp::TsMicroseconds>(st, now) << std::endl;
  g_loop->run_after(interval_ms, std::bind(func_in_loop, interval_ms));
  last_mp[interval_ms] = now;
}


void func_run_at() {
  Timestamp now = Timestamp::now();
  LOG_INFO << "func_run at differ: " <<
           Timestamp::differ<Timestamp::TsMicroseconds>(st, now) << std::endl;
}

int main(int argc, char *argv[]) {
  LOG_INFO << "test time queue";
  penduo::EventLoop loop;
  g_loop = &loop;

  st = Timestamp::now();
  for (auto interval : {3, 5, 7}) {
    const int ms = interval * 1000;
    last_mp[ms] = Timestamp::now();
    loop.run_after(ms, std::bind(func_in_loop, ms));
  }

  loop.run_at(Timestamp::add<Timestamp::TsMilliseconds>(st, 25), func_run_at);

  loop.loop();

  return 0;
}