//
// Created by poilk on 2022/6/28.
//

#include "Logger.h"
#include <string>
#include <mutex>

INITIALIZE_EASYLOGGINGPP;

namespace penduo {

void Logger::Init() {
  static std::once_flag init_flag;
  auto init_fuc = [](){
    el::Configurations c;
    c.setToDefault();
    c.parseFromText("*GLOBAL:\n"
                    " FORMAT                = [%datetime][%level] %fbase:%line %msg\n"
                    " TO_FILE               = false\n"
                    " TO_STANDARD_OUTPUT    = true\n"
                    " SUBSECOND_PRECISION   = 6");
    el::Loggers::reconfigureAllLoggers(c);
  };
  std::call_once(init_flag, init_fuc);
}

thread_local char t_errnobuf[512];
//thread_local char t_time[64];
//thread_local time_t t_last_second;

const char *strerror_tl(int saved_errno){
  strerror_r(saved_errno, t_errnobuf, sizeof t_errnobuf);
  return t_errnobuf;
}

} // penduo