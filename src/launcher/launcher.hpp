#pragma once

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <string>

using namespace ftxui;

class Launcher {
private:
  enum class Tab {
    PROMPT,
    MENU,
  };

  ScreenInteractive screen;

  Tab tab = Tab::PROMPT;
  int tabIndex;

  std::string username;

  void setTab(Tab tab);
  void generateUsername();
  Component getPrompt();
  Component getMenu();

public:
  Launcher();
  void render();
};
