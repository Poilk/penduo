//
// Created by poilk on 2022/6/28.
//

#ifndef PENDUO_SRC_NET_CALLBACKS_H_
#define PENDUO_SRC_NET_CALLBACKS_H_

#include <functional>
#include <memory>
#include "base/Timestamp.h"

namespace penduo{

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void (const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void()> CloseCallback;

typedef std::function<void()> TimerCallback;

typedef std::function<void (const TcpConnectionPtr &, char *, size_t)> MessageCallback;

} // penduo

#endif //PENDUO_SRC_NET_CALLBACKS_H_
