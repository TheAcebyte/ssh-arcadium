#include "text-cycler.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <cassert>
#include <chrono>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

TextCycler::TextCycler(ScreenInteractive &screen, int delay,
                       std::initializer_list<std::string> list) {
  assert(delay > 0);
  std::vector<std::string> values(list);
  isRunning = true;

  thread = std::thread([this, &screen, delay, values] {
    int index = 0;
    while (isRunning) {
      index = (index + 1) % values.size();
      value = values[index];
      screen.PostEvent(Event::Custom);
      std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
  });
}

TextCycler::~TextCycler() {
  isRunning = false;
  if (thread.joinable()) {
    thread.join();
  }
}

std::string TextCycler::getValue() const { return value; }

std::shared_ptr<TextCycler>
TextCycler::createCycler(ScreenInteractive &screen, int delay,
                         std::initializer_list<std::string> list) {
  return std::make_shared<TextCycler>(screen, delay,
                                      std::initializer_list<std::string>(list));
}

std::shared_ptr<TextCycler>
TextCycler::createEllipsis1(ScreenInteractive &screen) {
  return createCycler(screen, 300, {"   ", ".  ", ".. ", "..."});
}

std::shared_ptr<TextCycler>
TextCycler::createEllipsis2(ScreenInteractive &screen) {
  return createCycler(screen, 500, {"   ", "..."});
}
