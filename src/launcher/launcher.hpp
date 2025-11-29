#pragma once

#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

class Launcher {
  ScreenInteractive screen;

public:
  Launcher();

  void start();
};
