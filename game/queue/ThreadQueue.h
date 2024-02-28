// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef TEMPLATE_THREADQUEUE_H
#define TEMPLATE_THREADQUEUE_H
#include <condition_variable>
#include <deque>
#include <mutex>

template<class T>
class ThreadQueue {
  std::deque<T> queue;
  mutable std::mutex data_mutex;
  std::condition_variable data_cond_var;
public:
  ThreadQueue() = default;
  ~ThreadQueue() = default;
  ThreadQueue(const ThreadQueue&) = delete;
  ThreadQueue& operator= (const ThreadQueue&) = delete;

  size_t get_size() const {
    std::unique_lock<std::mutex> lg{data_mutex};
    return queue.size();
  }

  void push(const T& val) {
    {
      std::unique_lock<std::mutex> lg{data_mutex};
      queue.push_back(val);
    }
    data_cond_var.notify_one();
  }
  void push(T&& val) {
    {
      std::unique_lock<std::mutex> lg{data_mutex};
      queue.push_back(std::move(val));
    }
    data_cond_var.notify_one();
  }

  T pop () {
    std::unique_lock<std::mutex> lck(data_mutex);
    data_cond_var.wait(lck, [this](){return !queue.empty();});
    T res = queue.front();
    queue.pop_front();
    return res;
  }

  bool try_pop(T& value)
  {
    std::lock_guard<std::mutex> lk(data_mutex);
    if(queue.empty())
      return false;
    value=std::move(queue.front());
    queue.pop_front();
    return true;
  }

};

#endif //TEMPLATE_THREADQUEUE_H


