#include "game.h"

#include <raylib.h>

#include "comp/sprite_renderer.h"
#include "comp/transform.h"
#include "sys/render.h"

namespace pong {

entt::registry &Game::GetRegistry() noexcept { return registry_; }

void Game::run() noexcept {
  InitWindow(settings_.window_width, settings_.window_height, kTitle);

  // Main loop
  while (!WindowShouldClose()) {
    auto const delta_time = GetFrameTime();

    Update(delta_time);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    Render();

    EndDrawing();
  }
}

void Game::Update(float const delta_time) noexcept {}

void Game::Render() noexcept { sys::RenderSprites(registry_); }

Game::Game(Game::Settings const settings) noexcept : settings_(settings) {
  // Create player
  auto player = registry_.create();
  registry_.emplace<comp::Transform>(player, Vector2(100.0F, 100.0F));
  registry_.emplace<comp::SpriteRenderer>(player, Vector2(28.0F, 96.0F), BLACK);
}

Game::~Game() { CloseWindow(); }

}  // namespace pong