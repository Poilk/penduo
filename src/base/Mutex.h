//
// Created by poilk on 2022/6/29.
//

#ifndef PENDUO_SRC_BASE_MUTEX_H_
#define PENDUO_SRC_BASE_MUTEX_H_

#include <mutex>

namespace penduo{

typedef std::mutex Mutex;
typedef std::lock_guard<Mutex> LockGuard;

} //namespace penduo


#endif //PENDUO_SRC_BASE_MUTEX_H_
