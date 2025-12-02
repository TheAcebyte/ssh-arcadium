#pragma once

#include "ftxui/dom/canvas.hpp"
#include "ftxui/screen/color.hpp"
#include "lib/types.hpp"

using namespace ftxui;

class BlockCanvas : public Canvas {
private:
  static constexpr st scale = 4;

  st width;
  st height;

  Color backgroundColor;

public:
  BlockCanvas(st width, st height, Color backgroundColor = Color::Default);

  st getWidth() const;
  st getHeight() const;
  st getScale() const;

  void fill(st x, st y, Color color);
  void fillRectangle(st x1, st y1, st x2, st y2, Color color);
  void clear();
};
