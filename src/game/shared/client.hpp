#pragma once

#include "lib/types.hpp"
#include <msgpack.hpp>

enum class ClientMessageType {
  PLAY_MESSAGE,
  MOVE_MESSAGE,
};

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
  namespace adaptor {

  template <> struct pack<ClientMessageType> {
    template <typename Stream>
    packer<Stream> &operator()(msgpack::packer<Stream> &o,
                               const ClientMessageType &v) const {
      return o.pack(static_cast<u8>(v));
    }
  };

  template <> struct convert<ClientMessageType> {
    msgpack::object const &operator()(msgpack::object const &o,
                                      ClientMessageType &v) const {
      v = static_cast<ClientMessageType>(o.as<u8>());
      return o;
    }
  };
  } // namespace adaptor
}
} // namespace msgpack

struct PlayMessage {
  u64 id;
  std::string username;

  MSGPACK_DEFINE(id, username);
};

struct MoveMessage {
  u64 id;
  std::string direction;

  MSGPACK_DEFINE(id, direction);
};

using ClientMessage = std::variant<PlayMessage, MoveMessage>;
