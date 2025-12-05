#include "game-ui.hpp"
#include "ftxui/dom/elements.hpp"
#include "game/client/game-state.hpp"
#include "game/shared/config.hpp"
#include "lib/types.hpp"
#include <string>

GameUI::GameUI(const GameState &state) : state(state) {}

Element GameUI::render(GameUIContext context) {
  if (state.getConnectionStatus() == ConnectionStatus::CONNECTING) {
    return hbox(text("Connecting to server"),
                text(context.ellipsis->getValue())) |
           center | color(Color::Green1) | bgcolor(Color::Grey3);
  }

  return hbox(vbox(renderLeaderboard(), renderEvents() | flex) |
                  size(WIDTH, EQUAL, Config::gridSize),
              separatorEmpty(), separatorEmpty(), renderGrid(),
              separatorEmpty(), separatorEmpty(),
              vbox(renderInfo() | flex, renderControls()) |
                  size(WIDTH, EQUAL, Config::gridSize)) |
         center | color(Color::Green1) | bgcolor(Color::Grey3);
}

Element GameUI::renderGrid() {
  auto grid = state.getGrid();
  gridCanvas.clear();

  for (pt x = 0; x < grid.size(); ++x) {
    for (pt y = 0; y < grid.size(); ++y) {
      if (grid.isSnake(x, y)) {
        u64 id = grid.getSnakeId(x, y);
        auto color = state.getPlayer(id).color;
        auto colors = Config::getColorPair(color);

        if (grid.isSnakeHead(x, y)) {
          gridCanvas.fill(x, y, colors.head);
        } else {
          gridCanvas.fill(x, y, colors.body);
        }
      } else if (grid.isFruit(x, y)) {
        gridCanvas.fill(x, y, Color::Red3);
      }
    }
  }

  st heightError = 2;
  st height = Config::gridSize + heightError;
  return window(text(" Grid "), canvas(&gridCanvas), LIGHT) |
         size(HEIGHT, EQUAL, height);
}

Element GameUI::renderLeaderboard() {
  auto leaderboard = state.getLeaderboard();
  Elements entries;
  st maxEntries = 10;
  st entriesCount = std::min(maxEntries, leaderboard.size());
  for (st i = 0; i < entriesCount; ++i) {
    auto entry = leaderboard[i];
    auto rank = std::to_string(i + 1);
    auto score = std::to_string(entry.score);
    entries.push_back(hbox(text(rank), text(". "), text(entry.username),
                           filler(), text(score) | color(Color::GreenYellow)));
  }

  st heightError = 1;
  st height = Config::gridSize / 2 + heightError;
  return window(text(" Leaderboard "),
                hbox(separatorEmpty(),
                     vbox(separatorEmpty(), entries, separatorEmpty()) | flex,
                     separatorEmpty()) |
                    flex,
                LIGHT) |
         size(HEIGHT, EQUAL, height);
};

Element GameUI::renderEvents() {
  auto events = state.getEvents();
  Elements entries;
  st maxEntries = 5;
  st entriesCount = std::min(maxEntries, events.size());
  for (st i = 0; i < entriesCount; ++i) {
    if (i > 0) {
      entries.push_back(separatorEmpty());
    }

    st index = events.size() - i - 1;
    entries.push_back(hbox(paragraph(events[index])));
  }

  return window(text(" Events "),
                hbox(separatorEmpty(),
                     vbox(separatorEmpty(), entries, separatorEmpty()) | flex,
                     separatorEmpty()) |
                    flex,
                LIGHT);
};

Element GameUI::renderInfo() {
  auto self = state.getPlayer(state.getId());

  std::string username = state.getUsername();
  std::string status = self.status == Status::ALIVE ? "Alive" : "Dead";
  std::string score = std::to_string(self.score);
  std::string playerCount = std::to_string(state.getPlayers().size());
  auto colors = Config::getColorPair(self.color);

  snakeCanvas.fill(0, 0, colors.head);
  for (int y = 1; y < 5; ++y) {
    snakeCanvas.fill(0, y, colors.body);
  }

  return window(
      text(" Info "),
      hbox(separatorEmpty(),
           vbox(separatorEmpty(),
                hbox(vbox(text("Name"), text("Status"), text("Score"),
                          text("Players")),
                     filler(),
                     vbox(text(username) | align_right,
                          text(status) | align_right, text(score) | align_right,
                          hbox(text(playerCount), text("/10")) | align_right)),
                separatorEmpty(), separator(), filler(),
                canvas(&snakeCanvas) | center, filler()) |
               flex,
           separatorEmpty()) |
          flex,
      LIGHT);
};

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
