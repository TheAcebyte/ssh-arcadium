#pragma once

#include "event.hpp"
#include "lib/message-queue.hpp"
#include "lib/types.hpp"
#include <boost/asio.hpp>
#include <functional>

namespace asio = boost::asio;

template <typename Message> struct SessionContext {
  u64 id;
  asio::io_context &io;
  asio::ip::tcp::socket socket;
  MessageQueue<Message> &incoming;
};

template <typename Connection, typename Message> class Session {
private:
  u64 id;
  MessageQueue<Message> outgoing;
  Connection connection;

  using EventHandler = std::function<void()>;

public:
  Session(SessionContext<Message> context)
      : id(context.id), connection({.io = context.io,
                                    .socket = std::move(context.socket),
                                    .incoming = context.incoming,
                                    .outgoing = outgoing}) {}

  ~Session() { stop(); }

  void start() { connection.start(); }
  void stop() { connection.stop(); }

  u64 getId() const { return id; }
  void send(Message message) { outgoing.push(message); }

  void addEventHandler(Event event, EventHandler handler) {
    connection.addEventHandler(event, handler);
  }
};
