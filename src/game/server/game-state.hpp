#pragma once

#include "game/shared/state.hpp"
#include "lib/point/point.hpp"
#include "lib/types.hpp"
#include <deque>
#include <set>

class GameState {
private:
  Grid grid;

  std::map<u64, Player> players;
  std::map<u64, Direction> inputs;
  // NOTE: Is it really worth the hassle?
  // bool playersUpdateFlag = false;

  using Snake = std::deque<Point>;
  std::map<u64, Snake> snakes;

  std::set<Point> fruits;
  st fruitTimer = Config::fruitSpawnRate;

  // TODO: Handle case when no spawnpoint is available
  Point getRandomSnakeSpawn();
  Point getRandomFruitSpawn();

  void spawnSnake(u64 id);
  void killSnake(u64 id);
  Point getNextSnakeHead(u64 id);

  void applyInputs();
  void spawnFruit();
  void moveSnakes();
  void collectFruit();
  void solveCollisions();
  void updateGrid();

public:
  void tick();

  Grid getGrid() const;
  std::map<u64, Player> getPlayers() const;

  void addPlayer(u64 id, std::string username);
  void removePlayer(u64 id);
  void setPlayerInput(u64 id, Direction direction);
  // bool consumePlayersUpdate();
};
