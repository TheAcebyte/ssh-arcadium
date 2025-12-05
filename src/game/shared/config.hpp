#pragma once

#include "ftxui/screen/color.hpp"
#include "lib/random.hpp"
#include "lib/types.hpp"
#include <msgpack.hpp>
#include <stdexcept>

namespace Config {
inline constexpr st gridSize = 26;
inline constexpr auto tickRate = std::chrono::milliseconds(100);

inline constexpr st fruitSpawnRate = 20;
inline constexpr st fruitLimit = 5;
inline constexpr st fruitSnakeThreshold = 1;

enum class Color {
  GREEN,
  BLUE,
  ORANGE,
  TOTAL_COUNT,
};

inline constexpr Color getRandomColor() {
  auto index = Random::get(0UL, static_cast<st>(Color::TOTAL_COUNT) - 1);
  return static_cast<Color>(index);
}

struct ColorPair {
  ftxui::Color head;
  ftxui::Color body;
};

inline constexpr ColorPair getColorPair(Color color) {
  switch (color) {
  case Color::GREEN:
    return {ftxui::Color::GreenYellow, ftxui::Color::Chartreuse3};

  case Color::BLUE:
    return {ftxui::Color::DeepSkyBlue1, ftxui::Color::DeepSkyBlue3};

  case Color::ORANGE:
    return {ftxui::Color::Orange1, ftxui::Color::DarkOrange3};

  default:
    throw std::runtime_error("Invalid color.");
  }
}
} // namespace Config

using SnakeColor = Config::Color;
MSGPACK_ADD_ENUM(SnakeColor);
