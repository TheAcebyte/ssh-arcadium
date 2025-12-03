#include "game-state.hpp"
#include "lib/types.hpp"
#include <string>
#include <string_view>

void GameState::setConnected() {
  connectionStatus = ConnectionStatus::CONNECTED;
};

ConnectionStatus GameState::getConnectionStatus() const {
  return connectionStatus;
};

u64 GameState::getId() const { return id; }
void GameState::setId(u64 id) { this->id = id; }

std::string_view GameState::getUsername() const { return username; }
void GameState::setUsername(std::string_view username) {
  this->username = username;
}
