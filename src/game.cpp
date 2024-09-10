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
    ClearBackground(RAYWHITE);

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
  constexpr auto kPaddleColor = BLACK;

  const auto paddle_y_position =
      (settings_.window_height / 2.0F) - (kPaddleSize.y / 2.0F);

  // Create player1
  auto player1 = registry_.create();
  registry_.emplace<comp::Name>(player1, "Player1");
  registry_.emplace<comp::Transform>(
      player1, Vector2(kPaddleXOffset, paddle_y_position));
  registry_.emplace<comp::Sprite>(player1, kPaddleSize, kPaddleColor);
  registry_.emplace<comp::Script>(
      player1, script_system_, "data/scripts/player.lua",
      std::unordered_map<std::string, sol::object>{
          {"isPlayerOne", sol::make_object(script_system_.GetState(), true)}});
  registry_.emplace<comp::Collider>(player1, kPaddleSize);

  // Create player2
  auto player2 = registry_.create();
  registry_.emplace<comp::Name>(player2, "Player2");
  registry_.emplace<comp::Transform>(
      player2, Vector2(settings_.window_width - kPaddleXOffset - kPaddleSize.x,
                       paddle_y_position));
  registry_.emplace<comp::Sprite>(player2, kPaddleSize, kPaddleColor);
  registry_.emplace<comp::Script>(
      player2, script_system_, "data/scripts/player.lua",
      std::unordered_map<std::string, sol::object>{
          {"isPlayerOne", sol::make_object(script_system_.GetState(), false)}});
  registry_.emplace<comp::Collider>(player2, kPaddleSize);

  constexpr auto kBallSize = Vector2(16, 16);

  auto ball = registry_.create();
  registry_.emplace<comp::Name>(ball, "Ball");
  registry_.emplace<comp::Transform>(
      ball,
      Vector2(settings_.window_width / 2.0, settings_.window_height / 2.0));
  registry_.emplace<comp::Sprite>(ball, kBallSize, RED);
  registry_.emplace<comp::Script>(ball, script_system_,
                                  "data/scripts/ball.lua");
  registry_.emplace<comp::Collider>(ball, kBallSize);
}

Game::~Game() { CloseWindow(); }

}  // namespace pong