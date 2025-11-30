#include "launcher.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
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
  auto container = Container::Tab({getPrompt(), getMenu()}, &tabIndex);
  auto renderer =
      Renderer(container, [container] { return container->Render(); });

  screen.Loop(container);
}

Component Launcher::getPrompt() {
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
  auto component = Container::Vertical({input});
  auto ellipsis = TextCycler::createEllipsis1(screen);
  auto renderer = Renderer(component, [input, ellipsis] {
    return vbox(
               window(hbox(filler(), text(" What's your username? "), filler()),
                      vbox(separatorEmpty(),
                           hbox(separatorEmpty(), separatorEmpty(),
                                input->Render(), separatorEmpty(),
                                separatorEmpty()),
                           separatorEmpty()),
                      LIGHT),
               separatorEmpty(),
               hbox(text("Press ENTER to continue"),
                    text(ellipsis->getValue())) |
                   hcenter) |
           size(WIDTH, EQUAL, 70) | center | color(Color::Green1) |
           bgcolor(Color::Grey3);
  });

  renderer |= CatchEvent([this](Event event) {
    if (event == Event::Return) {
      username = content;
      setTab(Tab::MENU);
      return true;
    }

    return event.is_character() && content.length() >= 16;
  });

  return renderer;
}

Component Launcher::getMenu() {
  auto renderer =
      Renderer([this] { return hbox(text("Welcome ahoy, "), text(username)); });
  return renderer;
}
