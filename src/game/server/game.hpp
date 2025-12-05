#include "game-server.hpp"
#include "game/server/game-state.hpp"
#include <chrono>

class Game {
  static constexpr auto tickRate = Config::tickRate;

  GameServer server;
  GameState state;

  void addEventHandlers();
  void processMessage(ClientMessage message);
  void broadcastState();

public:
  Game();
  void run();
};
