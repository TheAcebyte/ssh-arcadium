#include "launcher.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "shared/block-canvas/block-canvas.hpp"
#include "shared/random.hpp"
#include "shared/text-cycler/text-cycler.hpp"
#include "shared/types.hpp"
#include <initializer_list>
#include <memory>
#include <string>

using namespace ftxui;

Launcher::Launcher() : screen(ScreenInteractive::Fullscreen()) {
  setTab(tab);
  generateUsername();
}

void Launcher::setTab(Tab tab) {
  this->tab = tab;
  tabIndex = static_cast<int>(tab);
}

void Launcher::generateUsername() {
  u16 tag = Random::get<u16>();
  username = "Guest" + std::to_string(tag);
}

void Launcher::render() {
  auto container = Container::Tab({createPrompt(), createMenu()}, &tabIndex);
  screen.Loop(container);
}

Component Launcher::createPrompt() {
  static std::string content;

  InputOption options = InputOption::Default();
  options.transform = [](InputState state) {
    if (state.is_placeholder) {
      state.element |= color(Color::GreenYellow);
    }

    state.element |= bgcolor(Color::Default);
    return state.element;
  };

  auto input = Input(&content, username, options);
  auto ellipsis = TextCycler::createEllipsis1(screen);
  auto component = Renderer(input, [input, ellipsis] {
    return vbox(window(text(" What's your username? "),
                       vbox(separatorEmpty(),
                            hbox(separatorEmpty(), separatorEmpty(),
                                 input->Render(), separatorEmpty(),
                                 separatorEmpty()),
                            separatorEmpty()),
                       LIGHT),
                separatorEmpty(),
                hbox(text("Press "), text("ENTER") | color(Color::GreenYellow),
                     text(" to continue"), text(ellipsis->getValue())) |
                    hcenter) |
           size(WIDTH, EQUAL, 70) | center | color(Color::Green1) |
           bgcolor(Color::Grey3);
  });

  component |= CatchEvent([this](Event event) {
    if (event == Event::Return) {
      if (!content.empty()) {
        username = content;
      }

      setTab(Tab::MENU);
      return true;
    }

    return event.is_character() && content.length() >= 16;
  });

  return component;
}

BlockCanvas Launcher::getSnake() {
  auto snake = BlockCanvas(5, 5, Color::Grey3);
  snake.fill(0, 2, Color::DarkOliveGreen3);
  snake.fill(1, 2, Color::DarkGreen);
  snake.fill(2, 2, Color::DarkGreen);
  snake.fill(2, 1, Color::DarkGreen);
  snake.fill(2, 0, Color::DarkGreen);
  snake.fill(3, 0, Color::DarkGreen);
  snake.fill(4, 0, Color::DarkGreen);
  snake.fill(4, 1, Color::DarkGreen);
  snake.fill(4, 2, Color::DarkGreen);
  snake.fill(4, 3, Color::DarkGreen);
  snake.fill(4, 4, Color::DarkGreen);
  return snake;
}

Component Launcher::createMenu() {
  auto snake = getSnake();
  auto ellipsis = TextCycler::createEllipsis1(screen);
  auto component = Renderer([snake, ellipsis](bool) {
    return vbox(hbox(text("SSH Arcadium")) | hcenter | borderLight, filler(),
                window(text(" Snake "),
                       vbox(filler(), canvas(&snake) | hcenter, filler(),
                            text("0/10 players") | hcenter),
                       LIGHT) |
                    size(WIDTH, EQUAL, 50) | size(HEIGHT, EQUAL, 14) | hcenter,
                separatorEmpty(), filler(),
                hbox(hbox(text("ENTER") | color(Color::GreenYellow),
                          text(" - Play")),
                     filler(),
                     hbox(text("s") | color(Color::GreenYellow),
                          text(" - Settings")),
                     filler(),
                     hbox(text("q") | color(Color::GreenYellow),
                          text(" - Quit"))) |
                    size(WIDTH, EQUAL, 50) | hcenter) |
           size(HEIGHT, EQUAL, 25) | center | color(Color::Green1) |
           bgcolor(Color::Grey3);
  });

  component |= CatchEvent([&](Event event) {
    if (event == Event::Character('q')) {
      screen.Exit();
      return true;
    }

    return false;
  });

  return component;
}
