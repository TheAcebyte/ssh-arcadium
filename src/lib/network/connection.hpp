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

template <typename InMessage, typename OutMessage> struct ConnectionContext {
  asio::io_context &io;
  asio::ip::tcp::socket socket;
  MessageQueue<InMessage> &incoming;
  MessageQueue<OutMessage> &outgoing;
};

template <typename InMessage, typename OutMessage> class Connection {
private:
  static constexpr u8 headerSize = 4;
  static constexpr std::chrono::duration delay = std::chrono::milliseconds(10);

  asio::io_context &io;
  asio::ip::tcp::socket socket;
  MessageQueue<InMessage> &incoming;
  MessageQueue<OutMessage> &outgoing;

  u32 length = 0;
  std::vector<u8> payload;

  using Event = NetworkEvent;
  using EventHandler = std::function<void()>;
  using EventHandlerMap = std::map<Event, std::vector<EventHandler>>;
  EventHandlerMap handlers;

  void read() {
    asio::async_read(socket, asio::buffer(&length, headerSize),
                     [this](boost::system::error_code error, st) {
                       if (error) {
                         fire(Event::DISCONNECT);
                         return;
                       }

                       payload.resize(length);
                       asio::async_read(
                           socket, asio::buffer(payload),
                           [this](boost::system::error_code error, st) {
                             if (error) {
                               fire(Event::DISCONNECT);
                               return;
                             }

                             auto data =
                                 reinterpret_cast<const char *>(payload.data());
                             auto message = deserialize(data, length);
                             incoming.push(message);
                             fire(Event::MESSAGE);
                             read();
                           });
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
      return;
    }
    auto message = outgoing.pop();
    auto payload = serialize(message);

    u32 length = payload.size();
    auto data = std::make_shared<std::vector<u8>>(headerSize + length);
    std::memcpy(data->data(), &length, headerSize);
    std::copy(payload.begin(), payload.end(), data->begin() + headerSize);

    asio::async_write(socket, asio::buffer(*data),
                      [this, data](boost::system::error_code error, st) {
                        if (error) {
                          fire(Event::DISCONNECT);
                          return;
                        }

                        write();
                      });
  }

  void fire(Event event) {
    for (auto handler : handlers[event]) {
      handler();
    }
  }

protected:
  virtual std::vector<u8> serialize(OutMessage message) = 0;
  virtual InMessage deserialize(const char *data, st length) = 0;

public:
  Connection(ConnectionContext<InMessage, OutMessage> context)
      : io(context.io), socket(std::move(context.socket)),
        incoming(context.incoming), outgoing(context.outgoing) {}

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

  void connect(std::string_view host, st port) {
    asio::ip::tcp::resolver resolver(io);
    auto endpoint = resolver.resolve(host, std::to_string(port));
    asio::connect(socket, endpoint);
  }

  void addEventHandler(Event event, std::function<void()> handler) {
    handlers[event].push_back(handler);
  }
};
