//
// Created by poilk on 2022/6/28.
//

#include <iostream>

#include "base/Timestamp.h"

using namespace penduo;

int main(int argc, char *argv[]){
  Timestamp ts = Timestamp::now();
  Timestamp ts2 = Timestamp::add<Timestamp::TsS>(ts, 10);
  Timestamp ts3 = Timestamp::add<Timestamp::TsMS>(ts, 550);
  std::cout << Timestamp::differ<Timestamp::TsS>(ts, ts2) << std::endl;
  std::cout << Timestamp::differ<Timestamp::TsMS>(ts, ts2) << std::endl;
  std::cout << Timestamp::differ<Timestamp::TsS>(ts, ts3) << std::endl;
  std::cout << Timestamp::differ<Timestamp::TsMS>(ts, ts3) << std::endl;
  return 0;
}