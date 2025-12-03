#pragma once

#include "lib/types.hpp"
#include <msgpack.hpp>
#include <string>
#include <variant>

enum class MessageType {
  ID_MESSAGE,
  NAME_MESSAGE,
  MOVE_MESSAGE,
};

struct IdMessage {
  std::string id;

  MSGPACK_DEFINE(id);
};

struct NameMessage {
  std::string id;
  std::string name;

  MSGPACK_DEFINE(id, name);
};

struct MoveMessage {
  std::string id;
  u8 direction;

  MSGPACK_DEFINE(id, direction);
};

using Message = std::variant<IdMessage, NameMessage, MoveMessage>;
