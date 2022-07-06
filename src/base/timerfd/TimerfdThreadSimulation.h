//
// Created by poilk on 2022/7/5.
//

#ifndef PENDUO_SRC_BASE_TIMERFD_TIMERFDTHREADSIMULATION_H_
#define PENDUO_SRC_BASE_TIMERFD_TIMERFDTHREADSIMULATION_H_

#include "Timerfd.h"

namespace penduo {

class TimerfdThreadSimulation : public Timerfd {
 public:
  TimerfdThreadSimulation();
  ~TimerfdThreadSimulation()noexcept override;
  void read() override;
  void reset(Timestamp expiration) override;
  int get_fd() const override;
 private:
  int pipe_fd_[2];
  Timestamp expiration_ts_;
};

} // penduo

#endif //PENDUO_SRC_BASE_TIMERFD_TIMERFDTHREADSIMULATION_H_
