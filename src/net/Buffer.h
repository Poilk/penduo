//
// Created by poilk on 2022/7/10.
//

#ifndef PENDUO_SRC_NET_BUFFER_H_
#define PENDUO_SRC_NET_BUFFER_H_

#include <string>
#include "base/copyable.h"

namespace penduo {

class Buffer : copyable{
 public:
  std::string receive_as_string();
  size_t readable_bytes() const {return buffer_.size();}
  ssize_t read_fd(int fd, int *saved_error);
 private:
  //todo optimize
  std::string buffer_;
};

} // penduo

#endif //PENDUO_SRC_NET_BUFFER_H_
