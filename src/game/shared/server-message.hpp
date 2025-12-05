#pragma once

#include "lib/types.hpp"
#include "state.hpp"
#include <map>
#include <msgpack.hpp>
#include <string>

enum class ServerMessageType {
  ACK,
  ID,
  GRID,
  PLAYERS,
  EVENT,
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

struct EventsMessage {
  std::vector<std::string> events;

  MSGPACK_DEFINE(events);
};

using ServerMessage =
    std::variant<AckMessage, IdMessage, GridMessage, PlayersMessage, EventsMessage>;
