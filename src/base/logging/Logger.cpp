//
// Created by poilk on 2022/6/28.
//

#include "Logger.h"
#include <mutex>

INITIALIZE_EASYLOGGINGPP;

namespace penduo {

void Logger::Init() {
  static std::once_flag init_flag;
  auto init_fuc = [](){
    el::Configurations c;
    c.setToDefault();
    c.parseFromText("*GLOBAL:\n FORMAT = [%datetime][%level] %fbase:%line %msg");
    el::Loggers::reconfigureAllLoggers(c);
  };
  std::call_once(init_flag, init_fuc);
}

} // penduo