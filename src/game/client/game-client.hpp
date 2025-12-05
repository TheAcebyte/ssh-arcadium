#pragma once

#include "game/shared/client-message.hpp"
#include "game/shared/server-message.hpp"
#include "lib/network/client.hpp"
#include "lib/network/connection.hpp"
#include "lib/overload.hpp"
#include <msgpack.hpp>
#include <stdexcept>
#include <variant>

using ClientConnectionContext = ConnectionContext<ServerMessage, ClientMessage>;

class ClientConnection : public Connection<ServerMessage, ClientMessage> {
private:
  std::vector<u8> serialize(ClientMessage message) override {
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> packer(buffer);

    packer.pack_array(2);
    std::visit(Overload(
                   [&](PlayMessage message) {
                     packer.pack(ClientMessageType::PLAY);
                     packer.pack(message);
                   },
                   [&](MoveMessage message) {
                     packer.pack(ClientMessageType::MOVE);
                     packer.pack(message);
                   },
                   [&](RespawnMessage message) {
                     packer.pack(ClientMessageType::RESPAWN);
                     packer.pack(message);
                   }),
               message);

    return std::vector<u8>(buffer.data(), buffer.data() + buffer.size());
  }

  ServerMessage deserialize(const char *data, st length) override {
    msgpack::object_handle handle = msgpack::unpack(data, length);
    msgpack::object object = handle.get();

    if (object.type != msgpack::type::ARRAY || object.via.array.size != 2) {
      throw std::runtime_error("Invalid message format.");
    }

    auto type = object.via.array.ptr[0].as<ServerMessageType>();
    const msgpack::object &payload = object.via.array.ptr[1];

    switch (type) {
    case ServerMessageType::ACK:
      return payload.as<AckMessage>();

    case ServerMessageType::ID:
      return payload.as<IdMessage>();

    case ServerMessageType::GRID:
      return payload.as<GridMessage>();

    case ServerMessageType::PLAYERS:
      return payload.as<PlayersMessage>();

    case ServerMessageType::EVENT:
      return payload.as<EventsMessage>();

    default:
      throw std::runtime_error("Invalid message type.");
    }
  }

public:
  ClientConnection(ClientConnectionContext context)
      : Connection(std::move(context)) {}
};

using GameClient = Client<ClientConnection, ServerMessage, ClientMessage>;
