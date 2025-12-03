#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "game-state.hpp"
#include "game/shared/config.hpp"
#include "lib/block-canvas/block-canvas.hpp"

class GameUI {
private:
  const GameState &state;
  BlockCanvas grid =
      BlockCanvas(Config::gridSize, Config::gridSize, Color::Grey3);

  Element renderGrid();
  Element renderLeaderboard();
  Element renderEvents();
  Element renderInfo();
  Element renderControls();

public:
  GameUI(const GameState &);
  Element render();
};
