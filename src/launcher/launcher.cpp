#include "launcher.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "launcher-ui.hpp"
#include "lib/random.hpp"
#include "lib/text-cycler/text-cycler.hpp"
#include "lib/types.hpp"
#include <stdexcept>
#include <string>

using namespace ftxui;

Launcher::Launcher() { generateUsername(); }

void Launcher::run() {
  auto component = createComponent();
  addEventHandlers(component);
  screen.Loop(component);
}

void Launcher::generateUsername() {
  u16 tag = Random::get<u16>();
  username = "Guest" + std::to_string(tag);
}

Component Launcher::createComponent() {
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
  auto component = Renderer(input, [this, input, ellipsis]() {
    LauncherUIContext context = {
        .tab = tab,
        .input = input->Render(),
        .ellipsis = ellipsis,
    };

    return ui.render(context);
  });

  return component;
}

void Launcher::addEventHandlers(Component &component) {
  component |= CatchEvent([this](Event event) {
    switch (tab) {
    case LauncherTab::PROMPT:
      if (event == Event::Return) {
        if (!content.empty()) {
          username = content;
        }

        tab = LauncherTab::MENU;
        return true;
      }

      return event.is_character() && content.length() >= 16;

    case LauncherTab::MENU:
      if (event == Event::Character('q')) {
        screen.Exit();
        return true;
      }

      return false;

    default:
      throw std::runtime_error("Invalid launcher tab.");
    }
  });
}
