//
// Created by poilk on 2022/6/28.
//

#include <poll.h>
#include <cassert>

#include "net/Poller.h"
#include "net/EventLoop.h"
#include "net/Channel.h"
#include "base/logging/Logger.h"

namespace penduo {

Poller::Poller(EventLoop *loop) : owner_loop_(loop) {

}

Poller::~Poller() = default;

void Poller::poll(int timeout_ms, ChannelList &active_channels) {
  int num_events = ::poll(pollfds_.data(), pollfds_.size(), timeout_ms);
  if (num_events > 0) {
    LOG_TRACE << "eventloop " << owner_loop_ << " " << num_events << " events happend";
    fill_active_channels(num_events, active_channels);
  } else if (num_events == 0) {
    LOG_TRACE << "eventloop " << owner_loop_ << " nothing happened";
  } else {
    LOG_SYS_ERROR << "Poller::poll()";
  }
}

void Poller::fill_active_channels(int num_events, ChannelList &active_channels) {
  for (auto const &pfd : pollfds_) {
    //todo whate does revents < 0 mean?
    if (pfd.revents > 0) {
      num_events--;
      assert(channels_.count(pfd.fd) != 0);
      Channel *channel = channels_.at(pfd.fd);
      assert(channel->fd() == pfd.fd);
      channel->set_revents(pfd.revents);
      active_channels.push_back(channel);
    }
    if (!num_events) {
      break;
    }
  }
}

void Poller::update_channel(Channel *channel) {
  assert_in_loop_thread();
  assert(channel != nullptr);
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  if (channel->get_pollfds_index() < 0) {
    // a new one add to pollfds
    assert(channels_.count(channel->fd()) == 0);
    struct pollfd pfd{channel->fd(), static_cast<short>(channel->events()), 0};
    pollfds_.push_back(pfd);
    const int idx = static_cast<int>(pollfds_.size()) - 1;
    channel->set_pollfds_index(idx);
    channels_.insert({channel->fd(), channel});
  } else {
    // update existing one
    assert(channels_.count(channel->fd()) != 0);
    assert(channels_.at(channel->fd()) == channel);
    const int idx = channel->get_pollfds_index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    auto &pfd = pollfds_[idx];
    assert(pfd.fd == channel->fd() || pfd.fd == ~channel->fd());
    pfd = {channel->fd(), static_cast<short>(channel->events()), 0};
    if (channel->is_none_event()) {
      pfd.fd = ~channel->fd();
    }
  }
}

void Poller::remove_channel(Channel *channel) {
  assert_in_loop_thread();
  assert(channel);
  LOG_TRACE << "remove_channel fd = " << channel->fd();
  assert(channels_.count(channel->fd()));
  assert(channels_.at(channel->fd()) == channel);
  assert(channel->is_none_event());
  const int idx = channel->get_pollfds_index();
  assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
  auto &pfd = pollfds_[idx];
  assert(pfd.fd == ~channel->fd() && pfd.events == channel->events());
  assert(channels_.erase(channel->fd()) == 1);
  if(idx != pollfds_.size() - 1){
    std::swap(pollfds_[idx], pollfds_.back());
    const int update_fd = pollfds_[idx].fd < 0 ? ~pollfds_[idx].fd : pollfds_[idx].fd;
    assert(channels_.count(update_fd));
    channels_.at(update_fd)->set_pollfds_index(idx);
  }
  pollfds_.pop_back();
}

void Poller::assert_in_loop_thread() {
  owner_loop_->assert_in_loop_thread();
}

} // penduo