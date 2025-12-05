#pragma once

#include "game/shared/state.hpp"
#include "lib/types.hpp"
#include <map>
#include <string>
#include <string_view>

enum class ConnectionStatus {
  CONNECTING,
  CONNECTED,
};

struct LeaderboardEntry {
  std::string username;
  u64 score;
};

class GameState {
private:
  u64 id;
  std::string username;
  ConnectionStatus connectionStatus = ConnectionStatus::CONNECTING;

  Grid grid;
  std::map<u64, Player> players;
  std::vector<LeaderboardEntry> leaderboard;
  std::vector<std::string> events;

  void calculateLeaderboard();

public:
  u64 getId() const;
  void setId(u64 id);

  const std::string &getUsername() const;
  void setUsername(std::string_view username);

  ConnectionStatus getConnectionStatus() const;
  void setConnected();

  const Grid &getGrid() const;
  void setGrid(Grid grid);

  const Player &getPlayer(u64 id) const;
  const std::map<u64, Player> &getPlayers() const;
  void setPlayers(std::map<u64, Player> players);

  const std::vector<LeaderboardEntry> &getLeaderboard() const;

  const std::vector<std::string>& getEvents() const;
  void addEvents(std::vector<std::string> newEvents);
};
