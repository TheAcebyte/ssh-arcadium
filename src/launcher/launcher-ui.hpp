#pragma once

#include "ftxui/dom/node.hpp"
#include "shared/block-canvas/block-canvas.hpp"
#include "shared/text-cycler/text-cycler.hpp"

using namespace ftxui;

enum class LauncherTab {
  PROMPT,
  MENU,
};

struct LauncherUIContext {
  LauncherTab tab;
  Element input;
  std::shared_ptr<TextCycler> ellipsis;
};

class LauncherUI {
private:
  BlockCanvas snake = BlockCanvas(5, 5, Color::Grey3);
  void drawSnake();

  Element renderPrompt(Element input, std::shared_ptr<TextCycler> ellipsis);
  Element renderMenu();

public:
  LauncherUI();
  Element render(LauncherUIContext context);
};
