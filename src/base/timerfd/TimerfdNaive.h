//
// Created by poilk on 2022/7/4.
//

#ifndef PENDUO_SRC_NET_TIMERFD_TIMERFDNAIVE_H_
#define PENDUO_SRC_NET_TIMERFD_TIMERFDNAIVE_H_

#include "Timerfd.h"

namespace penduo {

class TimerfdNaive : public Timerfd{
 public:
  TimerfdNaive();
  ~TimerfdNaive() override;
  void read() override;
  void reset(Timestamp expiration) override;
  int get_fd()const override{ return fd_;};
 private:
  int fd_;
};

} // penduo

#endif //PENDUO_SRC_NET_TIMERFD_TIMERFDNAIVE_H_
