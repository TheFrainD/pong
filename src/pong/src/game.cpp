#include "game.h"

#include <core/comp/collider.h>
#include <core/comp/label.h>
#include <core/comp/name.h>
#include <core/comp/script.h>
#include <core/comp/sprite.h>
#include <core/comp/transform.h>
#include <core/sys/physics.h>
#include <core/sys/render.h>
#include <raylib.h>

namespace pong {

using namespace core;

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

void Game::Render() noexcept { sys::Render(registry_); }

Game::Game(Game::Settings const settings) noexcept
    : settings_(settings), script_system_(registry_, dispatcher_) {
  using namespace std::literals;

  constexpr auto kPaddleXOffset = 30.0F;
  constexpr auto kPaddleColor = RAYWHITE;
  constexpr Vector2 kPaddleSize = {14.0F, 96.0F};

  const float paddle_y_position =
      (settings_.window_height / 2.0F) - (kPaddleSize.y / 2.0F);

  // Create player1
  auto player1 = registry_.create();
  registry_.emplace<comp::Name>(player1, "Player1");
  registry_.emplace<comp::Transform>(
      player1, Vector2{kPaddleXOffset, paddle_y_position});
  registry_.emplace<comp::Sprite>(player1, kPaddleSize, kPaddleColor);
  comp::AddScript(
      registry_, script_system_, player1, "data/scripts/player.lua",
      {{"isPlayerOne", sol::make_object(script_system_.GetState(), true)}});
  registry_.emplace<comp::Collider>(player1, kPaddleSize);

  // Create player2
  auto player2 = registry_.create();
  registry_.emplace<comp::Name>(player2, "Player2");
  registry_.emplace<comp::Transform>(
      player2, Vector2{settings_.window_width - kPaddleXOffset - kPaddleSize.x,
                       paddle_y_position});
  registry_.emplace<comp::Sprite>(player2, kPaddleSize, kPaddleColor);
  comp::AddScript(
      registry_, script_system_, player2, "data/scripts/player.lua",
      {{"isPlayerOne", sol::make_object(script_system_.GetState(), false)}});
  registry_.emplace<comp::Collider>(player2, kPaddleSize);

  constexpr Vector2 kBallSize = {10.0F, 10.0F};

  auto ball = registry_.create();
  registry_.emplace<comp::Name>(ball, "Ball");
  registry_.emplace<comp::Transform>(
      ball,
      Vector2{settings_.window_width / 2.0F, settings_.window_height / 2.0F});
  registry_.emplace<comp::Sprite>(ball, kBallSize, RAYWHITE);
  comp::AddScript(registry_, script_system_, ball, "data/scripts/ball.lua");
  registry_.emplace<comp::Collider>(ball, kBallSize);

  constexpr Vector2 kSeparatorSize = {10.0F, 20.0F};
  constexpr auto kSepartorOffset = 10;
  const float kSeparatorXPos =
      (settings_.window_width / 2.0F) - (kSeparatorSize.x / 2.0F);
  const int kSeparatorCount =
      settings_.window_height / (kSeparatorSize.y + kSepartorOffset);

  for (int i = 0; i < kSeparatorCount; ++i) {
    auto separator = registry_.create();
    const float kSeparatorYPos = i * (kSeparatorSize.y + kSepartorOffset);
    registry_.emplace<comp::Transform>(separator,
                                       Vector2{kSeparatorXPos, kSeparatorYPos});
    registry_.emplace<comp::Sprite>(separator, kSeparatorSize, RAYWHITE);
  }

  constexpr auto kScoreYPosition = 20.0F;
  const auto kScoreXOffset = 40.0F;

  auto player1_score = registry_.create();
  registry_.emplace<comp::Name>(player1_score, "Player1Score");
  registry_.emplace<comp::Label>(player1_score, "0", 64, RAYWHITE);
  const auto kTextWidth = 36;
  registry_.emplace<comp::Transform>(
      player1_score,
      Vector2{(settings_.window_width / 2.0F) - kTextWidth - kScoreXOffset,
              kScoreYPosition});

  auto player2_score = registry_.create();
  registry_.emplace<comp::Name>(player2_score, "Player2Score");
  registry_.emplace<comp::Label>(player2_score, "0", 64, RAYWHITE);
  registry_.emplace<comp::Transform>(
      player2_score,
      Vector2{static_cast<float>((settings_.window_width / 2) + kScoreXOffset),
              kScoreYPosition});

  auto score_manager = registry_.create();
  registry_.emplace<comp::Name>(score_manager, "ScoreManager");
  comp::AddScript(registry_, script_system_, score_manager,
                  "data/scripts/score.lua");
}

Game::~Game() { CloseWindow(); }

}  // namespace pong