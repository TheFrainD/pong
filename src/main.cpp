#include "../include/game.h"

int main(int argc, char *argv[]) {
  pong::Game::Settings settings = {};
  settings.window_width = 800;
  settings.window_height = 600;

  pong::Game game(settings);
  game.run();

  return 0;
}