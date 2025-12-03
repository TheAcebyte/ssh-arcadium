#pragma once

#include "lib/types.hpp"
#include <string>
#include <string_view>

enum class ConnectionStatus {
  CONNECTING,
  CONNECTED,
};

class GameState {
private:
  ConnectionStatus connectionStatus = ConnectionStatus::CONNECTING;

  u64 id;
  std::string username;

public:
  void setConnected();
  ConnectionStatus getConnectionStatus() const;

  u64 getId() const;
  void setId(u64 id);

  std::string_view getUsername() const;
  void setUsername(std::string_view username);
};
