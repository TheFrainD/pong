#include "game.h"

int main(int argc, char *argv[]) {
  using pong::Game;

  Game::Settings settings = {};
  settings.window_width = 800;
  settings.window_height = 600;

  Game game(settings);
  game.Run();

  return 0;
}