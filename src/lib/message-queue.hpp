#pragma once

#include <mutex>
#include <queue>
#include <stdexcept>

template <typename Message> class MessageQueue {
private:
  std::queue<Message> queue;
  std::mutex mutex;

public:
  void push(Message message) {
    std::lock_guard lock(mutex);
    queue.push(std::move(message));
  }

  Message pop() {
    std::lock_guard lock(mutex);
    if (queue.empty()) {
      throw std::runtime_error("Cannot pop from empty MessageQueue");
    }

    Message message = std::move(queue.front());
    queue.pop();
    return message;
  }

  bool empty() {
    std::lock_guard lock(mutex);
    return queue.empty();
  }
};
