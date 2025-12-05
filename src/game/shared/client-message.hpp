#pragma once

#include "game/shared/state.hpp"
#include "lib/types.hpp"
#include <msgpack.hpp>

enum class ClientMessageType {
  PLAY,
  MOVE,
  RESPAWN,
};

MSGPACK_ADD_ENUM(ClientMessageType);

struct PlayMessage {
  u64 id;
  std::string username;

  MSGPACK_DEFINE(id, username);
};

struct MoveMessage {
  u64 id;
  Direction direction;

  MSGPACK_DEFINE(id, direction);
};

struct RespawnMessage {
  u64 id;

  MSGPACK_DEFINE(id);
};

using ClientMessage = std::variant<PlayMessage, MoveMessage, RespawnMessage>;
