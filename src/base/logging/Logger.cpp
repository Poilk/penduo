//
// Created by poilk on 2022/6/28.
//

#include "Logger.h"
#include <mutex>

namespace penduo {

void Logger::Init() {
  static std::once_flag init_flag;
  auto init_fuc = [](){
    google::InitGoogleLogging("penduo");
    FLAGS_logtostderr = true;
    FLAGS_stderrthreshold = google::GLOG_INFO;
    LOG(INFO) << "in Logger::Init init_func";
  };
  std::call_once(init_flag, init_fuc);
}

} // penduo