#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "game-state.hpp"
#include "game/shared/config.hpp"
#include "lib/block-canvas/block-canvas.hpp"
#include "lib/text-cycler/text-cycler.hpp"
#include <memory>

struct GameUIContext {
  std::shared_ptr<TextCycler> ellipsis;
};

class GameUI {
private:
  const GameState &state;
  BlockCanvas gridCanvas =
      BlockCanvas(Config::gridSize, Config::gridSize, Color::Grey3);
  BlockCanvas snakeCanvas = BlockCanvas(1, 5, Color::Grey3);

  Element renderGrid();
  Element renderLeaderboard();
  Element renderEvents();
  Element renderInfo();
  Element renderControls();

public:
  GameUI(const GameState &);
  Element render(GameUIContext context);
};
