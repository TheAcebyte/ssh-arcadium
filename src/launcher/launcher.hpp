#pragma once

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "launcher-ui.hpp"

// TODO: Run games with execvp() through some static method
class Launcher {
private:
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  LauncherTab tab = LauncherTab::PROMPT;
  LauncherUI ui;

  std::string content;
  std::string username;
  void generateUsername();

  Component createComponent();
  void addEventHandlers(Component &component);

public:
  Launcher();
  void run();
};
