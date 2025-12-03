#pragma once

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "game-state.hpp"
#include "game-ui.hpp"
#include "game/shared/network.hpp"
#include <string_view>

class Game {
private:
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  GameClient client;
  GameState state;
  GameUI ui;

public:
  Game(std::string_view username);

  Component createComponent();
  void run();
};
