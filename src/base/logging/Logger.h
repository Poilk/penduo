//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_BASE_LOGGING_LOGGER_H_
#define PENDUO_SRC_BASE_LOGGING_LOGGER_H_

#include <glog/logging.h>

namespace penduo {

class Logger {
 public:
  static void Init();
};

//#define LOG_DEBUG

#define LOG_INFO Logger::Init();LOG(INFO)
#define LOG_WARNING Logger::Init();LOG(WARNING)
#define LOG_ERROR Logger::Init();LOG(ERROR)
#define LOG_FATAL Logger::Init();LOG(FATAL)

} // penduo

#endif //PENDUO_SRC_BASE_LOGGING_LOGGER_H_
