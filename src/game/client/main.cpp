#include "game.hpp"
#include <stdexcept>

int main(int argc, char **argv) {
  if (argc < 2) {
    throw std::runtime_error("Name argument is unset.");
  }

  std::string username(argv[1]);
  Game game(username);
  game.run();

  return 0;
}
