#pragma once

#include "event.hpp"
#include "lib/message-queue.hpp"
#include "lib/types.hpp"
#include "session.hpp"
#include <atomic>
#include <format>
#include <map>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace asio = boost::asio;

template <typename Connection, typename Message> class Server {
private:
  asio::io_context io;
  asio::ip::tcp::acceptor acceptor;
  MessageQueue<Message> incoming;

  std::map<u64, std::shared_ptr<Session<Connection, Message>>> sessions;
  std::mutex mutex;
  u64 nextId = 0;

  std::thread thread;
  std::atomic<bool> running = false;

  using Event = NetworkEvent;
  using EventHandler = std::function<void(u64)>;
  using EventHandlerMap = std::map<Event, std::vector<EventHandler>>;
  EventHandlerMap handlers;

  void accept() {
    acceptor.async_accept([this](boost::system::error_code error,
                                 asio::ip::tcp::socket socket) {
      if (!error && running) {
        u64 id = nextId++;
        auto session = std::make_shared<Session<Connection, Message>>(
            SessionContext<Message>({.id = id,
                                     .io = io,
                                     .socket = std::move(socket),
                                     .incoming = incoming}));
        {
          std::lock_guard lock(mutex);
          sessions[id] = session;
        }

        session->start();
        session->addEventHandler(Event::DISCONNECT,
                                 [this, id] { fire(Event::DISCONNECT, id); });
        session->addEventHandler(Event::MESSAGE,
                                 [this, id] { fire(Event::MESSAGE, id); });
        fire(Event::CONNECT, id);
        accept();
      }
    });
  }

  void disconnect(u64 id) {
    std::lock_guard lock(mutex);
    if (!sessions.count(id)) {
      auto error = std::format("Could not find session with ID: {}", id);
      throw std::runtime_error(error);
    }

    auto session = sessions[id];
    session->stop();
    sessions.erase(id);
  }

  void fire(Event event, u64 initiatorId) {
    for (auto handler : handlers[event]) {
      handler(initiatorId);
    }
  }

public:
  Server(st port)
      : acceptor(io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

  ~Server() { stop(); }

  void start() {
    if (running) {
      return;
    }

    running = true;
    accept();
    thread = std::thread([this] { io.run(); });
  }

  void stop() {
    if (!running) {
      return;
    }

    running = false;
    {
      std::lock_guard lock(mutex);
      for (auto &[id, session] : sessions) {
        session->stop();
      }
      sessions.clear();
    }

    acceptor.close();
    io.stop();
    if (thread.joinable()) {
      thread.join();
    }
  }

  void send(u64 id, Message message) {
    std::lock_guard lock(mutex);
    if (!sessions.count(id)) {
      auto error = std::format("Could not find session with ID: {}", id);
      throw std::runtime_error(error);
    }

    auto session = sessions[id];
    session->send(message);
  }

  void broadcast(Message message) {
    std::lock_guard lock(mutex);
    for (auto &[id, session] : sessions) {
      session->send(message);
    }
  }

  void broadcastExcept(Message message, u64 excludedId) {
    std::lock_guard lock(mutex);
    for (auto &[id, session] : sessions) {
      if (id == excludedId) {
        continue;
      }

      session->send(message);
    }
  }

  bool empty() { return incoming.empty(); }
  Message process() { return incoming.pop(); }

  void addEventHandler(Event event, EventHandler handler) {
    handlers[event].push_back(handler);
  }
};
