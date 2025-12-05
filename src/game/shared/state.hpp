#pragma once

#include "config.hpp"
#include "lib/point/point.hpp"
#include "lib/types.hpp"
#include <msgpack.hpp>
#include <string>

enum class Status {
  ALIVE,
  DEAD,
};

MSGPACK_ADD_ENUM(Status);

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
};

using SnakeDirection = Direction;
Direction getOppositeDirection(Direction direction);

MSGPACK_ADD_ENUM(Direction);

struct Player {
  u64 id;
  std::string username;
  Status status;
  u64 score;
  SnakeColor color;
  Direction direction;

  MSGPACK_DEFINE(id, username, status, score, color, direction);
};

enum class CellType {
  EMPTY,
  FRUIT,
  SNAKE_HEAD,
  SNAKE_BODY,
};

MSGPACK_ADD_ENUM(CellType);

struct Cell {
  CellType type;
  u64 snakeId;

  MSGPACK_DEFINE(type, snakeId);
};

// TODO: Add out of bounds checks
class Grid {
private:
  static constexpr pt n = Config::gridSize;
  std::array<Cell, n * n> data;

public:
  Grid();
  pt size() const;

  void clear();

  bool isEmpty(pt x, pt y) const;
  bool isFruit(pt x, pt y) const;
  bool isSnake(pt x, pt y) const;
  bool isSnakeBody(pt x, pt y) const;
  bool isSnakeHead(pt x, pt y) const;
  u64 getSnakeId(pt x, pt y) const;

  void setEmpty(pt x, pt y);
  void setFruit(pt x, pt y);
  void setSnakeBody(pt x, pt y, u64 id);
  void setSnakeHead(pt x, pt y, u64 id);

  bool isEmpty(Point point) const;
  bool isFruit(Point point) const;
  bool isSnake(Point point) const;
  bool isSnakeBody(Point point) const;
  bool isSnakeHead(Point point) const;
  u64 getSnakeId(Point point) const;

  void setEmpty(Point point);
  void setFruit(Point point);
  void setSnakeBody(Point point, u64 id);
  void setSnakeHead(Point point, u64 id);

  MSGPACK_DEFINE(data);
};
