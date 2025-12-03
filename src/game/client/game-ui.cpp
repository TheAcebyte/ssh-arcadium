#include "game-ui.hpp"
#include "ftxui/dom/elements.hpp"
#include "game/client/game-state.hpp"
#include "game/shared/config.hpp"
#include "lib/types.hpp"

GameUI::GameUI(const GameState &state) : state(state) {}

Element GameUI::render(GameUIContext context) {
  if (state.getConnectionStatus() == ConnectionStatus::CONNECTING) {
    return hbox(text("Connecting to server"),
                text(context.ellipsis->getValue())) |
           center | color(Color::Green1) | bgcolor(Color::Grey3);
  }

  return hbox(vbox(renderLeaderboard() | flex, renderEvents() | flex) |
                  size(WIDTH, EQUAL, Config::gridSize),
              separatorEmpty(), separatorEmpty(), renderGrid(),
              separatorEmpty(), separatorEmpty(),
              vbox(renderInfo() | flex, renderControls()) |
                  size(WIDTH, EQUAL, Config::gridSize)) |
         center | color(Color::Green1) | bgcolor(Color::Grey3);
}

Element GameUI::renderGrid() {
  st heightError = 2;
  st height = Config::gridSize + heightError;
  return window(text(" Grid "), canvas(&grid), LIGHT) |
         size(HEIGHT, EQUAL, height);
}

Element GameUI::renderLeaderboard() {
  return window(text(" Leaderboard "), vbox(), LIGHT);
};

Element GameUI::renderEvents() {
  return window(text(" Events "), vbox(), LIGHT);
};

Element GameUI::renderInfo() { return window(text(" Info "), vbox(), LIGHT); };

Element GameUI::renderControls() {
  return window(
      text(" Controls "),
      vbox(separatorEmpty(),
           hbox(separatorEmpty(),
                vbox(text("Up"), text("Left"), text("Down"), text("Right"),
                     filler(), text("Respawn"), text("Quit")),
                filler(),
                vbox(text("w"), text("a"), text("s"), text("d"),
                     separatorEmpty(), text("r"), text("q")) |
                    color(Color::GreenYellow),
                separatorEmpty()),
           separatorEmpty()),
      LIGHT);
};
