#include "launcher-ui.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "shared/text-cycler/text-cycler.hpp"
#include <memory>

LauncherUI::LauncherUI() { drawSnake(); }

void LauncherUI::drawSnake() {
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
}

Element LauncherUI::renderPrompt(Element input,
                                 std::shared_ptr<TextCycler> ellipsis) {
  return vbox(window(text(" What's your username? "),
                     vbox(separatorEmpty(),
                          hbox(separatorEmpty(), separatorEmpty(), input,
                               separatorEmpty(), separatorEmpty()),
                          separatorEmpty()),
                     LIGHT),
              separatorEmpty(),
              hbox(text("Press "), text("ENTER") | color(Color::GreenYellow),
                   text(" to continue"), text(ellipsis->getValue())) |
                  hcenter) |
         size(WIDTH, EQUAL, 70) | center | color(Color::Green1) |
         bgcolor(Color::Grey3);
}

Element LauncherUI::renderMenu() {
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
}

Element LauncherUI::render(LauncherUIContext context) {
  switch (context.tab) {
  case LauncherTab::PROMPT:
    return renderPrompt(context.input, context.ellipsis);

  case LauncherTab::MENU:
    return renderMenu();
  }
}
