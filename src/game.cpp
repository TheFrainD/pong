#include "game.h"

#include <raylib.h>

#include "comp/collider.h"
#include "comp/name.h"
#include "comp/script.h"
#include "comp/sprite.h"
#include "comp/transform.h"
#include "sys/physics.h"
#include "sys/render.h"

namespace pong {

entt::registry &Game::GetRegistry() noexcept { return registry_; }

void Game::Run() noexcept {
  InitWindow(settings_.window_width, settings_.window_height, kTitle);
  SetExitKey(KEY_NULL);

  script_system_.OnStart();

  // Main loop
  while (!WindowShouldClose()) {
    auto const delta_time = GetFrameTime();

    Update(delta_time);

    BeginDrawing();
    ClearBackground(BLACK);

    Render();

    EndDrawing();
  }
}

void Game::Update(float const delta_time) noexcept {
  script_system_.Update(delta_time);
  sys::PhysicsUpdate(registry_, dispatcher_);

  dispatcher_.update();
}

void Game::Render() noexcept { sys::RenderSprites(registry_); }

Game::Game(Game::Settings const settings) noexcept
    : settings_(settings), script_system_(registry_, dispatcher_) {
  using namespace std::literals;

  constexpr auto kPaddleXOffset = 30.0F;
  constexpr auto kPaddleSize = Vector2(14.0F, 96.0F);
  constexpr auto kPaddleColor = RAYWHITE;

  const auto paddle_y_position =
      (settings_.window_height / 2.0F) - (kPaddleSize.y / 2.0F);

  // Create player1
  auto player1 = registry_.create();
  registry_.emplace<comp::Name>(player1, "Player1");
  registry_.emplace<comp::Transform>(
      player1, Vector2(kPaddleXOffset, paddle_y_position));
  registry_.emplace<comp::Sprite>(player1, kPaddleSize, kPaddleColor);
  comp::AddScript(
      registry_, script_system_, player1, "data/scripts/player.lua",
      {{"isPlayerOne", sol::make_object(script_system_.GetState(), true)}});
  registry_.emplace<comp::Collider>(player1, kPaddleSize);

  // Create player2
  auto player2 = registry_.create();
  registry_.emplace<comp::Name>(player2, "Player2");
  registry_.emplace<comp::Transform>(
      player2, Vector2(settings_.window_width - kPaddleXOffset - kPaddleSize.x,
                       paddle_y_position));
  registry_.emplace<comp::Sprite>(player2, kPaddleSize, kPaddleColor);
  comp::AddScript(
      registry_, script_system_, player2, "data/scripts/player.lua",
      {{"isPlayerOne", sol::make_object(script_system_.GetState(), false)}});
  registry_.emplace<comp::Collider>(player2, kPaddleSize);

  constexpr auto kBallSize = Vector2(10, 10);

  auto ball = registry_.create();
  registry_.emplace<comp::Name>(ball, "Ball");
  registry_.emplace<comp::Transform>(
      ball,
      Vector2(settings_.window_width / 2.0, settings_.window_height / 2.0));
  registry_.emplace<comp::Sprite>(ball, kBallSize, RAYWHITE);
  comp::AddScript(registry_, script_system_, ball, "data/scripts/ball.lua");
  registry_.emplace<comp::Collider>(ball, kBallSize);

  constexpr auto kSeparatorSize = Vector2(10, 20);
  constexpr int kSepartorOffset = 10;
  const float kSeparatorXPos =
      (settings_.window_width / 2.0) - (kSeparatorSize.x / 2.0);
  const int kSeparatorCount =
      settings_.window_height / (kSeparatorSize.y + kSepartorOffset);

  for (int i = 0; i < kSeparatorCount; ++i) {
    auto separator = registry_.create();
    const float kSeparatorYPos = i * (kSeparatorSize.y + kSepartorOffset);
    registry_.emplace<comp::Transform>(separator,
                                       Vector2(kSeparatorXPos, kSeparatorYPos));
    registry_.emplace<comp::Sprite>(separator, kSeparatorSize, RAYWHITE);
  }
}

Game::~Game() { CloseWindow(); }

}  // namespace pong