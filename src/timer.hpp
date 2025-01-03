#ifndef TIMER_HPP
#define TIMER_HPP

#include <boost/asio.hpp>
#include <chrono>
#include <cstdint>
#include <iostream>

namespace asio = boost::asio;

template<typename Task>
class Timer
{
  asio::io_context& m_context;
  asio::steady_timer m_timer;
  Task m_task;
  uint64_t m_timeout;

 public:
  Timer(asio::io_context& io_context, Task task, uint64_t timeout)
      : m_context{io_context}
      , m_timer{io_context}
      , m_task{std::move(task)}
      , m_timeout{timeout}
  {
    handle_timeout();
  }

 private:
  void handle_timeout()
  {
    auto duration = std::chrono::seconds(m_timeout);
    m_timer.expires_after(duration);
    m_timer.async_wait([&](auto error_code) {
      m_task();
      m_timer.expires_after(duration);
      handle_timeout();
    });
  }
};

#endif
