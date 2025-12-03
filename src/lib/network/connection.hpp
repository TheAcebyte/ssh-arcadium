#pragma once

#include "event.hpp"
#include "lib/message-queue.hpp"
#include "lib/types.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string_view>
#include <vector>

namespace asio = boost::asio;

template <typename Message> struct ConnectionContext {
  asio::io_context &io;
  asio::ip::tcp::socket socket;
  MessageQueue<Message> &incoming;
  MessageQueue<Message> &outgoing;
};

template <typename Message> class Connection {
protected:
  static constexpr st size = 4096;
  static constexpr std::chrono::duration delay = std::chrono::milliseconds(10);

  asio::io_context &io;
  asio::ip::tcp::socket socket;
  MessageQueue<Message> &incoming;
  MessageQueue<Message> &outgoing;
  std::vector<u8> buffer;

  using Event = NetworkEvent;
  using EventHandler = std::function<void()>;
  using EventHandlerMap = std::map<Event, std::vector<EventHandler>>;
  EventHandlerMap handlers;

  void read() {
    socket.async_read_some(asio::buffer(buffer),
                           [this](boost::system::error_code error, st length) {
                             if (!error) {
                               auto data = reinterpret_cast<const char *>(
                                   buffer.data());
                               auto message = deserialize(data, length);
                               incoming.push(message);
                               fire(Event::MESSAGE);
                               read();
                             } else {
                               fire(Event::DISCONNECT);
                             }
                           });
  }

  void write() {
    if (outgoing.empty()) {
      auto timer = std::make_shared<asio::steady_timer>(io);
      timer->expires_after(delay);
      timer->async_wait([this, timer](boost::system::error_code error) {
        if (!error) {
          write();
        }
      });
    } else {
      auto message = outgoing.pop();
      auto data = std::make_shared<std::vector<u8>>(serialize(message));
      asio::async_write(socket, asio::buffer(*data),
                        [this, data](boost::system::error_code error, st) {
                          if (!error) {
                            write();
                          } else {
                            fire(Event::DISCONNECT);
                          }
                        });
    }
  }

  virtual std::vector<u8> serialize(const Message &message) = 0;
  virtual Message deserialize(const char *data, st length) = 0;

  void fire(Event event) {
    for (auto handler : handlers[event]) {
      handler();
    }
  }

public:
  Connection(ConnectionContext<Message> context)
      : io(context.io), socket(std::move(context.socket)),
        incoming(context.incoming), outgoing(context.outgoing) {
    buffer.resize(size);
  }

  virtual ~Connection() { stop(); }

  void start() {
    read();
    write();
  }

  void stop() {
    if (socket.is_open()) {
      socket.close();
    }
  }

  void connect(std::string_view host, std::string_view port) {
    asio::ip::tcp::resolver resolver(io);
    auto endpoint = resolver.resolve(host, port);
    asio::connect(socket, endpoint);
  }

  void addEventHandler(Event event, std::function<void()> handler) {
    handlers[event].push_back(handler);
  }
};
