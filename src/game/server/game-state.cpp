#include "game-state.hpp"
#include "game/shared/config.hpp"
#include "game/shared/state.hpp"
#include "lib/point/point.hpp"
#include "lib/random.hpp"
#include "lib/types.hpp"
#include <format>
#include <map>
#include <set>
#include <stdexcept>

void GameState::tick() {
  applyInputs();
  spawnFruit();
  moveSnakes();
  collectFruit();
  solveCollisions();
  updateGrid();
}

Grid GameState::getGrid() const { return grid; }
std::map<u64, Player> GameState::getPlayers() const { return players; }

void GameState::addPlayer(u64 id, std::string username) {
  if (players.contains(id)) {
    std::string error = std::format("Player with ID {} already exists.", id);
    throw std::runtime_error(error);
  }

  Player player = {
      .id = id,
      .username = std::move(username),
      .status = Status::ALIVE,
      .score = 0,
      .color = Config::getRandomColor(),
      .direction = Direction::UP,
  };

  players[id] = std::move(player);
  inputs[id] = Direction::UP;
  spawnPlayer(id);
}

void GameState::removePlayer(u64 id) {
  if (!players.contains(id)) {
    std::string error = std::format("Could not find player with ID {}.", id);
    throw std::runtime_error(error);
  }

  killPlayer(id);
  inputs.erase(id);
  players.erase(id);
}

void GameState::setPlayerInput(u64 id, Direction input) {
  const Player &player = players[id];
  if (player.status == Status::DEAD) {
    return;
  }

  Direction direction = player.direction;
  if (input == getOppositeDirection(direction)) {
    return;
  }

  inputs[id] = input;
}

void GameState::respawnPlayer(u64 id) {
  if (players[id].status == Status::ALIVE) {
    return;
  }

  spawnPlayer(id);
}

Point GameState::getRandomSnakeSpawn() {
  std::vector<Point> positions;
  for (pt x = 0; x < grid.size(); ++x) {
    for (pt y = 0; y < grid.size() - 1; ++y) {
      if (!grid.isEmpty(x, y) || !grid.isEmpty(x, y + 1)) {
        continue;
      }

      positions.emplace_back(x, y);
    }
  }

  st index = Random::get(0UL, positions.size() - 1);
  return positions[index];
}

Point GameState::getRandomFruitSpawn() {
  std::vector<Point> positions;
  for (pt x = 0; x < grid.size(); ++x) {
    for (pt y = 0; y < grid.size(); ++y) {
      if (!grid.isEmpty(x, y)) {
        continue;
      }

      positions.emplace_back(x, y);
    }
  }

  st index = Random::get(0UL, positions.size() - 1);
  return positions[index];
}

Point GameState::getNextSnakePosition(u64 id) {
  Snake &snake = snakes[id];
  Direction direction = players[id].direction;
  Point head = snake.front();

  switch (direction) {
  case Direction::UP:
    return Point(head.x, head.y - 1);

  case Direction::DOWN:
    return Point(head.x, head.y + 1);

  case Direction::LEFT:
    return Point(head.x - 1, head.y);

  case Direction::RIGHT:
    return Point(head.x + 1, head.y);

  default:
    throw std::runtime_error("Invalid direction.");
  }
}

void GameState::spawnPlayer(u64 id) {
  Snake &snake = snakes[id];
  Point head = getRandomSnakeSpawn();
  Point tail(head.x, head.y + 1);

  snake.push_back(head);
  snake.push_back(tail);
  grid.setSnakeHead(head, id);
  grid.setSnakeBody(tail, id);
  players[id].status = Status::ALIVE;
}

void GameState::killPlayer(u64 id) {
  snakes.erase(id);
  players[id].status = Status::DEAD;
  players[id].score = 0;
  players[id].color = Config::getRandomColor();
  players[id].direction = Direction::UP;
  inputs[id] = Direction::UP;
}

void GameState::applyInputs() {
  for (auto [id, input] : inputs) {
    players[id].direction = input;
  }
}

void GameState::spawnFruit() {
  if (snakes.size() < Config::fruitSnakeThreshold ||
      fruits.size() >= Config::fruitLimit) {
    return;
  }

  if (fruitTimer > 0) {
    --fruitTimer;
  } else {
    Point fruit = getRandomFruitSpawn();
    grid.setFruit(fruit);
    fruits.insert(fruit);
    fruitTimer = Config::fruitSpawnRate;
  }
}

void GameState::moveSnakes() {
  for (auto &[id, snake] : snakes) {
    Point newHead = getNextSnakePosition(id);
    snake.push_front(newHead);
  }
}

void GameState::collectFruit() {
  for (auto &[id, snake] : snakes) {
    Point head = snake.front();
    if (fruits.contains(head)) {
      ++players[id].score;
      fruits.erase(head);
    } else {
      snake.pop_back();
    }
  }
}

void GameState::solveCollisions() {
  std::set<u64> deadSnakes;

  for (auto &[id, snake] : snakes) {
    Point head = snake.front();
    if (head.x < 0 || head.x >= grid.size() || head.y < 0 ||
        head.y >= grid.size()) {
      deadSnakes.insert(id);
      continue;
    }

    for (auto &[otherId, otherSnake] : snakes) {
      st start = otherId == id ? 1 : 0;
      for (st i = start; i < otherSnake.size(); ++i) {
        Point otherPart = otherSnake[i];
        if (head == otherPart) {
          deadSnakes.insert(id);
          break;
        }
      }

      if (deadSnakes.contains(id)) {
        break;
      }
    }
  }

  for (u64 id : deadSnakes) {
    killPlayer(id);
  }
}

void GameState::updateGrid() {
  grid.clear();
  for (auto fruit : fruits) {
    grid.setFruit(fruit);
  }

  for (auto &[id, snake] : snakes) {
    Point head = snake.front();
    grid.setSnakeHead(head, id);
    for (st i = 1; i < snake.size(); ++i) {
      Point body = snake[i];
      grid.setSnakeBody(body, id);
    }
  }
}
