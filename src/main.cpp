#include "game.h"

#if defined(_WIN32) && !defined(_DEBUG)
int __stdcall WinMain(void *hInstance, void *hPrevInstance, char *lpCmdLine,
                      int nCmdShow) {
#else
int main(int argc, char **argv) {
#endif
  using pong::Game;

  Game::Settings settings = {};
  settings.window_width = 800;
  settings.window_height = 600;

  Game game(settings);
  game.Run();

  return 0;
}