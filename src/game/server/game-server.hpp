#pragma once

#include "game/shared/client-message.hpp"
#include "game/shared/server-message.hpp"
#include "lib/network/connection.hpp"
#include "lib/network/server.hpp"
#include "lib/overload.hpp"
#include "lib/types.hpp"
#include <msgpack.hpp>
#include <stdexcept>
#include <variant>

using ServerConnectionContext = ConnectionContext<ClientMessage, ServerMessage>;

class ServerConnection : public Connection<ClientMessage, ServerMessage> {
private:
  std::vector<u8> serialize(ServerMessage message) override {
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer(buffer);

    packer.pack_array(2);
    std::visit(Overload(
                   [&](AckMessage message) {
                     packer.pack(ServerMessageType::ACK);
                     packer.pack(message);
                   },
                   [&](IdMessage message) {
                     packer.pack(ServerMessageType::ID);
                     packer.pack(message);
                   },
                   [&](GridMessage message) {
                     packer.pack(ServerMessageType::GRID);
                     packer.pack(std::move(message));
                   },
                   [&](PlayersMessage message) {
                     packer.pack(ServerMessageType::PLAYERS);
                     packer.pack(std::move(message));
                   },
                   [&](EventsMessage message) {
                     packer.pack(ServerMessageType::EVENT);
                     packer.pack(std::move(message));
                   }),
               message);

    return std::vector<u8>(buffer.data(), buffer.data() + buffer.size());
  }

  ClientMessage deserialize(const char *data, st length) override {
    msgpack::object_handle handle = msgpack::unpack(data, length);
    msgpack::object object = handle.get();

    if (object.type != msgpack::type::ARRAY || object.via.array.size != 2) {
      throw std::runtime_error("Invalid message format.");
    }

    auto type = object.via.array.ptr[0].as<ClientMessageType>();
    const msgpack::object &payload = object.via.array.ptr[1];

    switch (type) {
    case ClientMessageType::PLAY:
      return payload.as<PlayMessage>();

    case ClientMessageType::MOVE:
      return payload.as<MoveMessage>();

    case ClientMessageType::RESPAWN:
      return payload.as<RespawnMessage>();

    default:
      throw std::runtime_error("Invalid message type.");
    }
  }

public:
  ServerConnection(ServerConnectionContext context)
      : Connection(std::move(context)) {}
};

using GameServer = Server<ServerConnection, ClientMessage, ServerMessage>;
