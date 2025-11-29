#include "launcher.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <chrono>
#include <string>
#include <thread>

using namespace ftxui;

Launcher::Launcher() : screen(ScreenInteractive::Fullscreen()) {}

void Launcher::start() {
  int counter = 0;

  auto component = Renderer([&] {
    return hbox(text("Counter: "), text(std::to_string(counter)) | bold) |
           center;
  });

  std::thread([&] {
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      ++counter;
      screen.PostEvent(Event::Custom);
    }
  }).detach();

  screen.Loop(component);
}
