#pragma once

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "launcher-ui.hpp"
#include <string_view>

enum class LauncherGame {
  NONE,
  SNAKE,
};

class Launcher {
private:
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  LauncherTab tab = LauncherTab::PROMPT;
  LauncherUI ui;

  LauncherGame game = LauncherGame::NONE;
  std::string content;
  std::string username;
  void generateUsername();

  Component createComponent();
  void addEventHandlers(Component &component);

public:
  Launcher();

  void run();
  void launch();
};
