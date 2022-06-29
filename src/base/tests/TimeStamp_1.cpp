//
// Created by poilk on 2022/6/28.
//

#include <iostream>

#include "base/Timestamp.h"

using namespace penduo;

int main(int argc, char *argv[]){
  Timestamp ts = Timestamp::now();
  Timestamp ts2 = Timestamp::add<Timestamp::TsSeconds>(ts, 10);
  Timestamp ts3 = Timestamp::add<Timestamp::TsMilliseconds>(ts, 550);
  std::cout << Timestamp::differ<Timestamp::TsSeconds>(ts, ts2) << std::endl;
  std::cout << Timestamp::differ<Timestamp::TsMilliseconds>(ts, ts2) << std::endl;
  std::cout << Timestamp::differ<Timestamp::TsSeconds>(ts, ts3) << std::endl;
  std::cout << Timestamp::differ<Timestamp::TsMilliseconds>(ts, ts3) << std::endl;
  return 0;
}