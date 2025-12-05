#include "game-state.hpp"
#include "lib/types.hpp"
#include <iterator>
#include <string>
#include <string_view>

u64 GameState::getId() const { return id; }
void GameState::setId(u64 id) { this->id = id; }

const std::string &GameState::getUsername() const { return username; }
void GameState::setUsername(std::string_view username) {
  this->username = username;
}

ConnectionStatus GameState::getConnectionStatus() const {
  return connectionStatus;
};

void GameState::setConnected() {
  connectionStatus = ConnectionStatus::CONNECTED;
};

const Grid &GameState::getGrid() const { return grid; }

void GameState::setGrid(Grid grid) { this->grid = std::move(grid); }

const Player &GameState::getPlayer(u64 id) const { return players.at(id); }

const std::map<u64, Player> &GameState::getPlayers() const { return players; }

void GameState::setPlayers(std::map<u64, Player> players) {
  this->players = std::move(players);
  calculateLeaderboard();
}

const std::vector<LeaderboardEntry> &GameState::getLeaderboard() const {
  return leaderboard;
}

void GameState::calculateLeaderboard() {
  std::vector<LeaderboardEntry> newLeaderboard;
  for (auto &[id, player] : players) {
    if (player.status == Status::DEAD) {
      continue;
    }

    LeaderboardEntry entry = {
        .username = player.username,
        .score = player.score,
    };

    newLeaderboard.push_back(entry);
  }

  std::ranges::sort(newLeaderboard,
                    [](auto a, auto b) { return a.score > b.score; });

  leaderboard = std::move(newLeaderboard);
}

const std::vector<std::string> &GameState::getEvents() const { return events; }

void GameState::addEvents(std::vector<std::string> newEvents) {
  auto begin = std::make_move_iterator(newEvents.begin());
  auto end = std::make_move_iterator(newEvents.end());
  events.insert(events.end(), begin, end);
}
