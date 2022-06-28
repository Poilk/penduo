//
// Created by poilk on 2022/6/27.
//


#include <unistd.h>
#include "net/EventLoop.h"
#include <thread>
#include <iostream>

using namespace penduo;

EventLoop *g_loop;

void threadFunc(){
  g_loop->loop();
}

int main(int argc, char *argv[]){
  EventLoop loop;
  g_loop = &loop;

  std::thread thr(threadFunc);
  thr.join();

  return 0;
}
