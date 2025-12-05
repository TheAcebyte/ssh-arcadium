#pragma once

#include "lib/types.hpp"
#include "state.hpp"
#include <map>
#include <msgpack.hpp>

enum class ServerMessageType {
  ACK,
  ID,
  GRID,
  PLAYERS,
};

MSGPACK_ADD_ENUM(ServerMessageType);

enum class AckType {
  PLAY,
  MOVE,
};

MSGPACK_ADD_ENUM(AckType);

struct AckMessage {
  AckType ack;

  MSGPACK_DEFINE(ack);
};

struct IdMessage {
  u64 id;

  MSGPACK_DEFINE(id);
};

struct GridMessage {
  Grid grid;

  MSGPACK_DEFINE(grid);
};

struct PlayersMessage {
  std::map<u64, Player> players;

  MSGPACK_DEFINE(players);
};

using ServerMessage =
    std::variant<AckMessage, IdMessage, GridMessage, PlayersMessage>;
