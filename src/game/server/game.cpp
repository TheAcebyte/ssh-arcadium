#include "game.hpp"
#include "game/shared/client-message.hpp"
#include "game/shared/server-message.hpp"
#include "lib/network/event.hpp"
#include "lib/overload.hpp"
#include <chrono>
#include <iostream>
#include <thread>

Game::Game() : server(4000) { addEventHandlers(); }

void Game::run() {
  server.start();
  while (true) {
    auto startTime = std::chrono::steady_clock::now();

    while (!server.empty()) {
      auto message = server.pop();
      processMessage(message);
    }

    state.tick();
    broadcastState();

    auto elapsedTime = std::chrono::steady_clock::now() - startTime;
    if (elapsedTime < tickRate) {
      std::this_thread::sleep_for(tickRate - elapsedTime);
    }
  }
}

void Game::addEventHandlers() {
  server.addEventHandler(NetworkEvent::CONNECT, [this](u64 id) {
    auto message = IdMessage(id);
    server.send(id, message);
  });

  server.addEventHandler(NetworkEvent::DISCONNECT,
                         [this](u64 id) { state.removePlayer(id); });
}

void Game::processMessage(ClientMessage message) {
  std::visit(
      Overload(
          [this](PlayMessage message) {
            state.addPlayer(message.id, message.username);

            GridMessage grid(state.getGrid());
            server.send(message.id, std::move(grid));

            PlayersMessage players(state.getPlayers());
            server.send(message.id, std::move(players));

            AckMessage ack(AckType::PLAY);
            server.send(message.id, ack);
          },
          [this](MoveMessage message) {
            state.setPlayerInput(message.id, message.direction);
          },
          [this](RespawnMessage message) { state.respawnPlayer(message.id); }),
      message);
}

void Game::broadcastState() {
  GridMessage grid(state.getGrid());
  server.broadcast(std::move(grid));

  PlayersMessage players(state.getPlayers());
  server.broadcast(std::move(players));

  if (state.hasUnreadEvents()) {
    EventsMessage events(state.readEvents());
    server.broadcast(std::move(events));
  }
}
