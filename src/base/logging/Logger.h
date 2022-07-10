//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_BASE_LOGGING_LOGGER_H_
#define PENDUO_SRC_BASE_LOGGING_LOGGER_H_

#include "easyloggingpp/easylogging++.h"
#include <thread>

namespace penduo {

class Logger {
 public:
  static void Init();
};

//#define LOG_DEBUG
//thread::id std::this_thread::get_id() noexcept {}

#define LOG_TRACE Logger::Init();LOG(INFO) << std::this_thread::get_id() << ' '
#define LOG_DEBUG Logger::Init();LOG(INFO) << std::this_thread::get_id() << ' '
#define LOG_INFO Logger::Init();LOG(INFO) << std::this_thread::get_id() << ' '
#define LOG_WARNING Logger::Init();LOG(WARNING) << std::this_thread::get_id() << ' '
#define LOG_ERROR Logger::Init();LOG(ERROR) << std::this_thread::get_id() << ' '
#define LOG_FATAL Logger::Init();LOG(FATAL) << std::this_thread::get_id() << ' '

#define LOG_SYS_ERROR LOG(ERROR) << std::this_thread::get_id() << ' '
#define LOG_SYS_FATAL LOG(FATAL) << std::this_thread::get_id() << ' '

const char * strerror_tl(int saved_error);

} // penduo

#endif //PENDUO_SRC_BASE_LOGGING_LOGGER_H_
