#include "block-canvas.hpp"
#include "ftxui/dom/canvas.hpp"

using namespace ftxui;

BlockCanvas::BlockCanvas(st width, st height, Color backgroundColor)
    : Canvas(scale * width, scale * height), width(width), height(height),
      backgroundColor(backgroundColor) {
  clear();
}

st BlockCanvas::getWidth() const { return width; }
st BlockCanvas::getHeight() const { return height; }
st BlockCanvas::getScale() const { return scale; }

void BlockCanvas::fill(st x, st y, Color color) {
  for (st di = 0; di < scale; ++di) {
    st i = scale * x + di;
    for (st dj = 0; dj < scale; ++dj) {
      st j = scale * y + dj;
      DrawBlock(i, j, true, color);
    }
  }
}

void BlockCanvas::fillRectangle(st x1, st y1, st x2, st y2, Color color) {
  for (st i = x1; i <= x2; ++i) {
    for (st j = y1; j <= y2; ++j) {
      fill(i, j, color);
    }
  }
}

void BlockCanvas::clear() {
  fillRectangle(0, 0, width, height, backgroundColor);
}
