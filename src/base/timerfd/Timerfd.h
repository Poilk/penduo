//
// Created by poilk on 2022/7/4.
//

#ifndef PENDUO_SRC_NET_TIMERFD_TIMERFD_H_
#define PENDUO_SRC_NET_TIMERFD_TIMERFD_H_

#include "base/Timestamp.h"
#include "base/noncopyable.h"

namespace penduo {

class Timerfd : noncopyable{
 public:
  Timerfd() = default;
  virtual ~Timerfd() = default;
  virtual int get_fd()const = 0;
  virtual void read() = 0;
  virtual void reset(Timestamp expiration) = 0;
 protected:
  static struct timespec how_much_time_from_now(Timestamp when);
};

} // penduo

#endif //PENDUO_SRC_NET_TIMERFD_TIMERFD_H_
