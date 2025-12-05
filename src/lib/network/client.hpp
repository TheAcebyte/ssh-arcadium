#pragma once

#include "event.hpp"
#include "lib/message-queue.hpp"
#include "lib/types.hpp"
#include <atomic>
#include <boost/asio.hpp>
#include <functional>
#include <thread>

namespace asio = boost::asio;

template <typename Connection, typename InMessage, typename OutMessage>
class Client {
private:
  asio::io_context io;
  MessageQueue<InMessage> incoming;
  MessageQueue<OutMessage> outgoing;
  Connection connection;

  std::thread thread;
  std::atomic<bool> running = false;

  using Event = NetworkEvent;
  using EventHandler = std::function<void()>;

public:
  Client()
      : connection({.io = io,
                    .socket = asio::ip::tcp::socket(io),
                    .incoming = incoming,
                    .outgoing = outgoing}) {}

  ~Client() { stop(); }

  void connect(std::string_view host, st port) {
    if (running) {
      return;
    }

    running = true;
    connection.connect(host, port);
    connection.start();

    thread = std::thread([this] { io.run(); });
  }

  void stop() {
    if (!running) {
      return;
    }

    running = false;
    connection.stop();
    io.stop();
    if (thread.joinable()) {
      thread.join();
    }
  }

  void send(OutMessage message) { outgoing.push(std::move(message)); }
  bool empty() { return incoming.empty(); }
  InMessage pop() { return incoming.pop(); }

  void addEventHandler(Event event, EventHandler handler) {
    connection.addEventHandler(event, handler);
  }
};
