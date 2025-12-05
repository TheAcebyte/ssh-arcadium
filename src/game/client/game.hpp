#pragma once

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "game-client.hpp"
#include "game-state.hpp"
#include "game-ui.hpp"
#include "game/shared/server-message.hpp"
#include <string_view>

class Game {
private:
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  GameClient client;
  GameState state;
  GameUI ui;

  void addNetworkEventHandlers();
  Component createComponent();
  void addInputEventHandlers(Component &component);
  void processMessage(ServerMessage message);

public:
  Game(std::string_view username);
  void run();
};
