#include "game.h"

#include <raylib.h>

#include "comp/name.h"
#include "comp/script.h"
#include "comp/sprite.h"
#include "comp/transform.h"
#include "sys/render.h"

namespace pong {

entt::registry &Game::GetRegistry() noexcept { return registry_; }

void Game::Run() noexcept {
  InitWindow(settings_.window_width, settings_.window_height, kTitle);
  SetExitKey(KEY_NULL);

  script_system_.OnStart(registry_);

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
  script_system_.Update(registry_, delta_time);
}

void Game::Render() noexcept { sys::RenderSprites(registry_); }

Game::Game(Game::Settings const settings) noexcept
    : settings_(settings), script_system_(registry_) {
  using namespace std::literals;

  constexpr auto kPaddleXOffset = 50.0F;
  constexpr auto kPaddleSize = Vector2(28.0F, 96.0F);
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
}

Game::~Game() { CloseWindow(); }

}  // namespace pong