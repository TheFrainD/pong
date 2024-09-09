#include "game.h"

#include <raylib.h>

#include "comp/paddle.h"
#include "comp/player.h"
#include "comp/script.h"
#include "comp/sprite.h"
#include "comp/transform.h"
#include "sys/player_controller.h"
#include "sys/render.h"

namespace pong {

entt::registry &Game::GetRegistry() noexcept { return registry_; }

void Game::run() noexcept {
  InitWindow(settings_.window_width, settings_.window_height, kTitle);

  sys::ScriptSystem::OnStart(registry_);

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

void Game::Update(float const delta_time) noexcept {
  sys::ScriptSystem::Update(registry_, delta_time);
  sys::PlayerControllerUpdate(registry_, delta_time, settings_.window_height);
}

void Game::Render() noexcept { sys::RenderSprites(registry_); }

Game::Game(Game::Settings const settings) noexcept : settings_(settings) {
  constexpr auto kPaddleXOffset = 50.0F;
  constexpr auto kPaddleSize = Vector2(28.0F, 96.0F);
  constexpr auto kPaddleColor = BLACK;

  constexpr auto kPlayerSpeed = 250.0F;

  const auto paddle_y_position =
      (settings_.window_height / 2.0F) - (kPaddleSize.y / 2.0F);

  // Create player1
  auto player1 = registry_.create();
  registry_.emplace<comp::Transform>(
      player1, Vector2(kPaddleXOffset, paddle_y_position));
  registry_.emplace<comp::Sprite>(player1, kPaddleSize, kPaddleColor);
  registry_.emplace<comp::Script>(
      player1, script_system_.GetState(),
      "/home/frain/projects/pong/data/scripts/player.lua");
  //  registry_.emplace<comp::Player>(player1, kPlayerSpeed);
  //  registry_.emplace<comp::Paddle>(player1, comp::Paddle::Side::kLeft);
  //
  //  // Create player2
  //  auto player2 = registry_.create();
  //  registry_.emplace<comp::Transform>(
  //      player2, Vector2(settings_.window_width - kPaddleXOffset -
  //      kPaddleSize.x,
  //                       paddle_y_position));
  //  registry_.emplace<comp::Sprite>(player2, kPaddleSize, kPaddleColor);
  //  registry_.emplace<comp::Player>(player2, kPlayerSpeed);
  //  registry_.emplace<comp::Paddle>(player2, comp::Paddle::Side::kRight);
}

Game::~Game() { CloseWindow(); }

}  // namespace pong