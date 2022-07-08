//
// Created by poilk on 2022/7/8.
//

#ifndef PENDUO_SRC_BASE_DAYTIME_H_
#define PENDUO_SRC_BASE_DAYTIME_H_

#include <string>

namespace penduo {

class Daytime {
 public:
  static std::string get_current_formatted_time();
};

} // penduo

#endif //PENDUO_SRC_BASE_DAYTIME_H_
