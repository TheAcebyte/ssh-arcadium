#pragma once

#include <mutex>
#include <optional>
#include <queue>

template <typename Message> class MessageQueue {
private:
  std::queue<Message> queue;
  std::mutex mutex;

public:
  MessageQueue();

  void push(Message message) {
    std::lock_guard lock(mutex);
    queue.push(std::move(message));
  }

  std::optional<Message> pop() {
    std::lock_guard lock(mutex);
    if (queue.empty()) {
      return std::nullopt;
    }

    Message message = std::move(queue.front());
    queue.pop();
    return message;
  }

  bool empty() const {
    std::lock_guard lock(mutex);
    return queue.empty();
  }
};
