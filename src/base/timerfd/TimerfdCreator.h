//
// Created by poilk on 2022/7/4.
//

#ifndef PENDUO_SRC_NET_TIMERFD_TIMERFDCREATOR_H_
#define PENDUO_SRC_NET_TIMERFD_TIMERFDCREATOR_H_

#include "Timerfd.h"

namespace penduo {

class TimerfdCreator {
 public:
  static Timerfd *new_default();
};

} // penduo

#endif //PENDUO_SRC_NET_TIMERFD_TIMERFDCREATOR_H_
