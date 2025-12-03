#include "game.hpp"
#include "lib/overload.hpp"
#include <chrono>
#include <iostream>
#include <thread>

Game::Game() : server(4000) { addNetworkEventHandlers(); }

void Game::run() {
  server.start();
  while (true) {
    auto startTime = std::chrono::steady_clock::now();

    while (!server.empty()) {
      auto message = server.pop();
      processMessage(message);
    }

    auto elapsedTime = std::chrono::steady_clock::now() - startTime;
    if (elapsedTime < tickRate) {
      std::this_thread::sleep_for(tickRate - elapsedTime);
    }
  }
}

void Game::addNetworkEventHandlers() {
  server.addEventHandler(NetworkEvent::CONNECT, [this](u64 id) {
    auto message = IdMessage(id);
    server.send(id, message);
    std::cout << "Received new connection with ID: " << id << std::endl;
  });
}

void Game::processMessage(const ClientMessage &message) {
  std::visit(Overload(
                 [this](const PlayMessage &in) {
                   AckMessage out(AckType::ACK_PLAY);
                   server.send(in.id, out);
                 },
                 [](const MoveMessage &in) {}),
             message);
}
