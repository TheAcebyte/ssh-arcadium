#pragma once

#include "ftxui/component/screen_interactive.hpp"
#include <atomic>
#include <initializer_list>
#include <memory>
#include <string>
#include <thread>

using namespace ftxui;

class TextCycler {
private:
  std::atomic<bool> running;
  std::thread thread;
  std::string value;

  static std::shared_ptr<TextCycler>
  createCycler(ScreenInteractive &screen, int delay,
               std::initializer_list<std::string> list);

public:
  TextCycler(ScreenInteractive &screen, int delay,
             std::initializer_list<std::string> list);
  ~TextCycler();

  TextCycler(const TextCycler &other) = delete;
  TextCycler &operator=(const TextCycler &other) = delete;

  std::string getValue() const;

  static std::shared_ptr<TextCycler> createEllipsis1(ScreenInteractive &screen);
  static std::shared_ptr<TextCycler> createEllipsis2(ScreenInteractive &screen);
};
