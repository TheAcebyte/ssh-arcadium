#include "game-server.hpp"
#include <chrono>

class Game {
  static constexpr auto tickRate = std::chrono::milliseconds(100);

  GameServer server;

  void addNetworkEventHandlers();
  void processMessage(const ClientMessage &message);

public:
  Game();
  void run();
};
