//
// Created by poilk on 2022/7/8.
//

#include <chrono>

#include "Daytime.h"

namespace penduo {

std::string Daytime::get_current_formatted_time() {

  auto now   = std::chrono::system_clock::now();
  auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
  auto now_s= std::chrono::duration_cast<std::chrono::seconds>(now_ms);

  std::time_t timet = now_s.count();
  int64_t milltime = now_ms.count() % 1000;
  tm cur_time{};
  localtime_r(&timet, &cur_time);

  char buffer[64];
  sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d.%03ld", cur_time.tm_year + 1900, cur_time.tm_mon + 1,
          cur_time.tm_mday, cur_time.tm_hour, cur_time.tm_min, cur_time.tm_sec, milltime);
  return buffer;

}
} // penduo