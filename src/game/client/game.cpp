#include "game.hpp"
#include "ftxui/component/component.hpp"
#include "game/shared/state.hpp"
#include "lib/text-cycler/text-cycler.hpp"

using namespace ftxui;

Game::Game(std::string_view username) : ui(state) {
  state.setUsername(username);
  addNetworkEventHandlers();
}

void Game::run() {
  client.connect("localhost", 4000);
  auto component = createComponent();
  addInputEventHandlers(component);
  screen.Loop(component);
}

void Game::addNetworkEventHandlers() {
  client.addEventHandler(NetworkEvent::MESSAGE,
                         [this]() { screen.PostEvent(Event::Custom); });
}

Component Game::createComponent() {
  auto ellipsis = TextCycler::createEllipsis1(screen);
  auto component = Renderer([this, ellipsis](bool) {
    while (!client.empty()) {
      auto message = client.pop();
      processMessage(message);
    }

    GameUIContext context = {.ellipsis = ellipsis};
    return ui.render(context);
  });

  return component;
}

void Game::addInputEventHandlers(Component &component) {
  component |= CatchEvent([this](Event event) {
    if (event == Event::Character('w')) {
      MoveMessage message(state.getId(), SnakeDirection::UP);
      client.send(message);
    }

    if (event == Event::Character('s')) {
      MoveMessage message(state.getId(), SnakeDirection::DOWN);
      client.send(message);
    }

    if (event == Event::Character('a')) {
      MoveMessage message(state.getId(), SnakeDirection::LEFT);
      client.send(message);
    }

    if (event == Event::Character('d')) {
      MoveMessage message(state.getId(), SnakeDirection::RIGHT);
      client.send(message);
    }

    return false;
  });
}

void Game::processMessage(ServerMessage message) {
  std::visit(Overload(
                 [this](IdMessage in) {
                   state.setId(in.id);

                   PlayMessage out(state.getId(),
                                   std::string(state.getUsername()));
                   client.send(out);
                 },
                 [this](AckMessage message) {
                   if (message.ack == AckType::PLAY) {
                     state.setConnected();
                   }
                 },
                 [this](GridMessage message) {
                   state.setGrid(std::move(message.grid));
                 },
                 [this](PlayersMessage message) {
                   state.setPlayers(std::move(message.players));
                 }),
             message);
}
