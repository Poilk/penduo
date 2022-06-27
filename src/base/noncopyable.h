//
// Created by poilk on 2022/6/27.
//

#ifndef PENDUO_SRC_BASE_NONCOPYABLE_H_
#define PENDUO_SRC_BASE_NONCOPYABLE_H_

namespace penduo {
  class noncopyable{
   public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
   protected:
    noncopyable() = default;
    ~noncopyable() = default;
  };
} //namespace penduo

#endif //PENDUO_SRC_BASE_NONCOPYABLE_H_
