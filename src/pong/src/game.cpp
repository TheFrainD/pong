#include "game.h"

#include <core/comp/collider.h>
#include <core/comp/label.h>
#include <core/comp/name.h>
#include <core/comp/script/builder.h>
#include <core/comp/script/script.h>
#include <core/comp/sprite.h>
#include <core/comp/transform.h>
#include <core/sys/physics.h>
#include <core/sys/render.h>
#include <fmt/format.h>
#include <raylib.h>

#include <entt/entity/fwd.hpp>

#include "core/entity/builder.h"
#include "core/scene/scene.h"
#include "core/util/file_reader.h"

namespace pong {

void Game::Run() noexcept {
  InitWindow(settings_.window_width, settings_.window_height, kTitle);
  SetWindowSize(settings_.window_width, settings_.window_height);
  SetExitKey(KEY_NULL);

  running_ = true;

  scene_manager_.Transition("Menu");

  // Main loop
  while (running_) {
    auto const delta_time = GetFrameTime();

    Update(delta_time);

    BeginDrawing();
    ClearBackground(BLACK);

    Render();

    EndDrawing();

    if (WindowShouldClose()) {
      running_ = false;
    }
  }
}

void Game::Update(float const delta_time) noexcept {
  scene_manager_.GetCurrentScene()->Update(delta_time);
  core::sys::PhysicsUpdate(scene_manager_.GetRegistry(),
                           scene_manager_.GetDispatcher());
}

void Game::Render() noexcept {
  core::sys::Render(scene_manager_.GetRegistry());
}

Game::Game(Game::Settings const settings) noexcept : settings_(settings) {
  using namespace std::literals;
  using core::util::ReadFileToString;

  auto main_scene_setup = [this](core::scene::Scene &scene) {
    constexpr auto kPaddleXOffset = 30.0F;
    constexpr auto kPaddleColor = RAYWHITE;
    constexpr Vector2 kPaddleSize = {14.0F, 96.0F};

    const float paddle_y_position =
        (settings_.window_height / 2.0F) - (kPaddleSize.y / 2.0F);

    auto entity_builder = core::entity::EntityBuilder(
        "Player1", scene.GetRegistry(), scene.GetScriptSystem());

    entity_builder
        .AddComponent<core::comp::Transform>(
            Vector2{kPaddleXOffset, paddle_y_position})
        .AddComponent<core::comp::Sprite>(kPaddleSize, kPaddleColor)
        .AddComponent<core::comp::Collider>(kPaddleSize)
        .CreateScript("data/scripts/player.lua")
        .AddParameter("isPlayerOne", true)
        .RegisterScript();

    entity_builder.New("Player2")
        .AddComponent<core::comp::Transform>(
            Vector2{settings_.window_width - kPaddleXOffset - kPaddleSize.x,
                    paddle_y_position})
        .AddComponent<core::comp::Sprite>(kPaddleSize, kPaddleColor)
        .AddComponent<core::comp::Collider>(kPaddleSize)
        .CreateScript("data/scripts/player.lua")
        .AddParameter("isPlayerOne", false)
        .RegisterScript();

    constexpr Vector2 kBallSize = {10.0F, 10.0F};

    entity_builder.New("Ball")
        .AddComponent<core::comp::Transform>(Vector2{
            settings_.window_width / 2.0F, settings_.window_height / 2.0F})
        .AddComponent<core::comp::Sprite>(kBallSize, RAYWHITE)
        .AddComponent<core::comp::Collider>(kBallSize)
        .CreateScript("data/scripts/ball.lua")
        .RegisterScript();

    constexpr Vector2 kSeparatorSize = {10.0F, 20.0F};
    constexpr auto kSepartorOffset = 10;
    const float separator_x_pos =
        (settings_.window_width / 2.0F) - (kSeparatorSize.x / 2.0F);
    const int separator_count =
        settings_.window_height / (kSeparatorSize.y + kSepartorOffset);

    for (int i = 0; i < separator_count; ++i) {
      const float separator_y_pos = i * (kSeparatorSize.y + kSepartorOffset);
      entity_builder.New(fmt::format("Separator {}", i))
          .AddComponent<core::comp::Transform>(
              Vector2{separator_x_pos, separator_y_pos})
          .AddComponent<core::comp::Sprite>(kSeparatorSize, RAYWHITE);
    }

    auto create_score = [this, &entity_builder](int i) {
      constexpr auto kScoreYPosition = 20.0F;
      const auto score_x_offset = 40.0F;
      const auto text_width = 36.0F;

      entity_builder.New(fmt::format("Player{}Score", i))
          .AddComponent<core::comp::Label>("0", 64, RAYWHITE)
          .AddComponent<core::comp::Transform>(Vector2{
              (settings_.window_width / 2.0F) - (text_width * (2.0F - i)) +
                  (((i * 2.0F) - 3.0F) * score_x_offset),
              kScoreYPosition});
    };

    for (const auto i : {1, 2}) {
      create_score(i);
    }

    entity_builder.New("ScoreManager")
        .CreateScript("data/scripts/score.lua")
        .RegisterScript();
  };

  scene_manager_.CreateScene(
      "Menu",
      [this](core::scene::Scene &scene) {
        auto text_width = MeasureText("PONG", 100);
        auto entity_builder =
            core::entity::EntityBuilder("Title", scene.GetRegistry(),
                                        scene.GetScriptSystem())
                .AddComponent<core::comp::Transform>(Vector2{
                    (settings_.window_width / 2.0F) - (text_width / 2.0F),
                    (settings_.window_height / 2.0F) - 100.0F})
                .AddComponent<core::comp::Label>("PONG", 100, RAYWHITE);

        text_width = MeasureText("Press 'Enter' to start", 32);
        entity_builder.New("Hint")
            .AddComponent<core::comp::Transform>(
                Vector2{(settings_.window_width / 2.0F) - (text_width / 2.0F),
                        (settings_.window_height / 2.0F)})
            .AddComponent<core::comp::Label>("Press 'Enter' to start", 32,
                                             RAYWHITE);
      },
      [this](core::scene::Scene &, float) {
        if (IsKeyDown(KEY_ENTER)) {
          scene_manager_.Transition("Main");
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
          running_ = false;
        }
      });

  scene_manager_.CreateScene("Main", main_scene_setup,
                             [this](core::scene::Scene &, float) {
                               if (IsKeyDown(KEY_ESCAPE)) {
                                 scene_manager_.Transition("Menu");
                               }
                             });
}

Game::~Game() { CloseWindow(); }

}  // namespace pong
