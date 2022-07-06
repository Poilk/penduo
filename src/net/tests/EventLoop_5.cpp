//
// Created by poilk on 2022/7/6.
//

#include <unistd.h>

#include "net/EventLoop.h"

penduo::EventLoop *g_loop;
int g_flag = 0;

void run4() {
  printf("run4(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->quit();
}

void run3() {
  printf("run3(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->run_after(3000, run4);
  g_flag = 3;
}

void run2() {
  printf("run2(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->queue_in_loop(run3);
}

void run1() {
  g_flag = 1;
  printf("run1(): pid = %d, flag = %d\n", getpid(), g_flag);
  g_loop->run_in_loop(run2);
  g_flag = 2;
}

int main(int argc, char *argv[]) {
  printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);

  penduo::EventLoop loop;
  g_loop = &loop;

  loop.run_after(2000, run1);
  loop.loop();
  printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);
}

