//
// Created by poilk on 2022/7/4.
//

#include "TimerfdCreator.h"
#if PENDUO_USE_NAIVE_TIMERFD
#include "TimerfdNaive.h"
#else
#include "TimerfdThreadSimulation.h"
#endif

namespace penduo {

Timerfd *TimerfdCreator::new_default() {
#if PENDUO_USE_NAIVE_TIMERFD
  return new TimerfdNaive{};
#else
  return new TimerfdThreadSimulation{};
#endif
}
} // penduo