//
// Created by poilk on 2022/7/10.
//

#include <unistd.h>
#include <sys/uio.h>
#include <cstring>
#include <cassert>
#include "Buffer.h"

namespace penduo {

//todo optimize
std::string Buffer::receive_as_string() {
  std::string ret;
  std::swap(ret, buffer_);
  return ret;
}

ssize_t Buffer::read_fd(int fd, int *saved_error) {
  char extra_buffer[65536]{};
  struct iovec vec[1];
  vec[0].iov_base = extra_buffer;
  vec[0].iov_len = sizeof extra_buffer;
  ssize_t ret = readv(fd, vec, 1);
  if (ret < 0){
    *saved_error = errno;
  } else{
    assert(static_cast<size_t>(ret) == strlen(extra_buffer));
    buffer_ += extra_buffer;
  }
  return ret;
}
} // penduo