#include "game.hpp"
#include "ftxui/component/component.hpp"
#include "lib/text-cycler/text-cycler.hpp"

using namespace ftxui;

Game::Game(std::string_view username) : ui(state) {
  state.setUsername(username);
  addNetworkEventHandlers();
}

void Game::run() {
  client.connect("localhost", 4000);
  auto component = createComponent();
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

void Game::processMessage(const ServerMessage &message) {
  std::visit(Overload(
                 [this](const IdMessage &in) {
                   state.setId(in.id);

                   PlayMessage out(state.getId(),
                                   std::string(state.getUsername()));
                   client.send(out);
                 },
                 [this](const AckMessage &message) {
                   if (message.ack == AckType::ACK_PLAY) {
                     state.setConnected();
                   }
                 }),
             message);
}
