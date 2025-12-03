#include "game.hpp"
#include "ftxui/component/component.hpp"

using namespace ftxui;

Game::Game(std::string_view username) : ui(state) {
  state.setUsername(username);
}

Component Game::createComponent() {
  auto component = Renderer([this](bool) { return ui.render(); });
  return component;
}

void Game::run() {
  auto component = createComponent();
  screen.Loop(component);
}
