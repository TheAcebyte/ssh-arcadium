#include "state.hpp"
#include <stdexcept>

Direction getOppositeDirection(Direction direction) {
  switch (direction) {
  case Direction::UP:
    return Direction::DOWN;

  case Direction::DOWN:
    return Direction::UP;

  case Direction::LEFT:
    return Direction::RIGHT;

  case Direction::RIGHT:
    return Direction::LEFT;

  default:
    throw std::runtime_error("Invalid direction.");
  }
}

Grid::Grid() { clear(); }

pt Grid::size() const { return n; }

void Grid::clear() {
  for (st x = 0; x < n; ++x) {
    for (st y = 0; y < n; ++y) {
      setEmpty(x, y);
    }
  }
}

bool Grid::isEmpty(pt x, pt y) const {
  return data[y * n + x].type == CellType::EMPTY;
}

bool Grid::isFruit(pt x, pt y) const {
  return data[y * n + x].type == CellType::FRUIT;
}

bool Grid::isSnake(pt x, pt y) const {
  return isSnakeHead(x, y) || isSnakeBody(x, y);
}

bool Grid::isSnakeHead(pt x, pt y) const {
  return data[y * n + x].type == CellType::SNAKE_HEAD;
}

bool Grid::isSnakeBody(pt x, pt y) const {
  return data[y * n + x].type == CellType::SNAKE_BODY;
}

u64 Grid::getSnakeId(pt x, pt y) const { return data[y * n + x].snakeId; }

void Grid::setEmpty(pt x, pt y) { data[y * n + x].type = CellType::EMPTY; }
void Grid::setFruit(pt x, pt y) { data[y * n + x].type = CellType::FRUIT; }

void Grid::setSnakeHead(pt x, pt y, u64 id) {
  Cell &cell = data[y * n + x];
  cell.type = CellType::SNAKE_HEAD;
  cell.snakeId = id;
}

void Grid::setSnakeBody(pt x, pt y, u64 id) {
  Cell &cell = data[y * n + x];
  cell.type = CellType::SNAKE_BODY;
  cell.snakeId = id;
}

bool Grid::isEmpty(Point point) const { return isEmpty(point.x, point.y); }
bool Grid::isFruit(Point point) const { return isFruit(point.x, point.y); }
bool Grid::isSnake(Point point) const { return isSnake(point.x, point.y); }

bool Grid::isSnakeHead(Point point) const {
  return isSnakeHead(point.x, point.y);
}

bool Grid::isSnakeBody(Point point) const {
  return isSnakeBody(point.x, point.y);
}

u64 Grid::getSnakeId(Point point) const { return getSnakeId(point.x, point.y); }

void Grid::setEmpty(Point point) { setEmpty(point.x, point.y); }
void Grid::setFruit(Point point) { setFruit(point.x, point.y); }

void Grid::setSnakeHead(Point point, u64 id) {
  setSnakeHead(point.x, point.y, id);
}

void Grid::setSnakeBody(Point point, u64 id) {
  setSnakeBody(point.x, point.y, id);
}
