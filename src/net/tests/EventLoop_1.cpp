//
// Created by poilk on 2022/6/27.
//

#include <unistd.h>
#include "EventLoop.h"
#include <thread>
#include <iostream>
#include <cassert>

using namespace penduo;

void threadFunc()
{
  //printf("threadFunc(): pid = %d, std_tid = %d, naive_tid = %d\n", getpid(), std::this_thread::get_id(), std::thread::native_handle());
  std::cout << "threadFunc(): pid = " << getpid() << ", tid = " << std::this_thread::get_id() << std::endl;

  assert(EventLoop::get_event_loop_of_current_thread() == nullptr);
  EventLoop loop;
  assert(EventLoop::get_event_loop_of_current_thread() == &loop);
  loop.loop();
}

int main(int argc, char *argv[]){
  std::thread t1(threadFunc);
  std::thread t2(threadFunc);
  std::thread t3(threadFunc);

  t1.join();
  t2.join();
  t3.join();
  threadFunc();

  return 0;
}