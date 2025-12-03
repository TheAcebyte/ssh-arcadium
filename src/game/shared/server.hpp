#pragma once

#include "lib/types.hpp"
#include <msgpack.hpp>

enum class ServerMessageType {
  ACK_MESSAGE,
  ID_MESSAGE,
};

enum class AckType {
  ACK_PLAY,
  ACK_MOVE,
};

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
  namespace adaptor {

  template <> struct pack<ServerMessageType> {
    template <typename Stream>
    packer<Stream> &operator()(msgpack::packer<Stream> &o,
                               const ServerMessageType &v) const {
      return o.pack(static_cast<u8>(v));
    }
  };

  template <> struct convert<ServerMessageType> {
    msgpack::object const &operator()(msgpack::object const &o,
                                      ServerMessageType &v) const {
      v = static_cast<ServerMessageType>(o.as<u8>());
      return o;
    }
  };

  template <> struct pack<AckType> {
    template <typename Stream>
    packer<Stream> &operator()(msgpack::packer<Stream> &o,
                               const AckType &v) const {
      return o.pack(static_cast<u8>(v));
    }
  };

  template <> struct convert<AckType> {
    msgpack::object const &operator()(msgpack::object const &o,
                                      AckType &v) const {
      v = static_cast<AckType>(o.as<u8>());
      return o;
    }
  };
  } // namespace adaptor
}
} // namespace msgpack

struct AckMessage {
  AckType ack;

  MSGPACK_DEFINE(ack);
};

struct IdMessage {
  u64 id;

  MSGPACK_DEFINE(id);
};

using ServerMessage = std::variant<AckMessage, IdMessage>;
