//
// Created by poilk on 2022/7/6.
//

#include <cstdio>
#include <unistd.h>
#include <thread>
#include "base/logging/Logger.h"
#include "net/EventLoop.h"
#include "net/EventLoopThread.h"

using namespace penduo;

void runInThread() {
  LOG_INFO << "runInThread(): pid = " << getpid() << " tid = " << std::this_thread::get_id();
}

int main() {
  LOG_INFO << "main(): pid = " << getpid() << " tid = " << std::this_thread::get_id();

  penduo::EventLoopThread loopThread;
  penduo::EventLoop *loop = loopThread.start_loop();
  loop->run_in_loop(runInThread);
  sleep(1);
  loop->run_after(2000, runInThread);
  sleep(3);
  loop->run_after(2000, runInThread);
  sleep(3);
  loop->quit();

  LOG_INFO << "exit main().";
}