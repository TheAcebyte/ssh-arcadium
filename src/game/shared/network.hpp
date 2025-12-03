#pragma once

#include "lib/network/client.hpp"
#include "lib/network/connection.hpp"
#include "lib/network/server.hpp"
#include "lib/overload.hpp"
#include "lib/types.hpp"
#include "message.hpp"

using GameConnectionContext = ConnectionContext<Message>;

class GameConnection : public Connection<Message> {
private:
  std::vector<u8> serialize(const Message &message) override {
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer(buffer);

    packer.pack_array(2);
    std::visit(Overload(
                   [&](IdMessage message) {
                     u8 index = static_cast<u8>(MessageType::ID_MESSAGE);
                     packer.pack(index);
                     packer.pack(message);
                   },
                   [&](NameMessage message) {
                     u8 index = static_cast<u8>(MessageType::NAME_MESSAGE);
                     packer.pack(index);
                     packer.pack(message);
                   },
                   [&](MoveMessage message) {
                     u8 index = static_cast<u8>(MessageType::MOVE_MESSAGE);
                     packer.pack(index);
                     packer.pack(message);
                   }),
               message);

    return std::vector<u8>(buffer.data(), buffer.data() + buffer.size());
  }

  Message deserialize(const char *data, st length) override {
    msgpack::object_handle handle = msgpack::unpack(data, length);
    msgpack::object object = handle.get();

    if (object.type != msgpack::type::ARRAY || object.via.array.size != 2) {
      throw std::runtime_error("Invalid message format");
    }

    u8 index = object.via.array.ptr[0].as<u8>();
    MessageType type = static_cast<MessageType>(index);
    const msgpack::object &payload = object.via.array.ptr[1];

    switch (type) {
    case MessageType::ID_MESSAGE:
      return payload.as<IdMessage>();

    case MessageType::NAME_MESSAGE:
      return payload.as<NameMessage>();

    case MessageType::MOVE_MESSAGE:
      return payload.as<MoveMessage>();

    default:
      throw std::runtime_error("Invalid message type");
    }
  }

public:
  GameConnection(GameConnectionContext context)
      : Connection(std::move(context)) {}
};

using GameClient = Client<GameConnection, Message>;
using GameServer = Server<GameConnection, Message>;
