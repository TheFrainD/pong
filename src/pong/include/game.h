#pragma once

#include <core/scene/scene_manager.h>
#include <core/sys/scripts.h>

#include <entt/entt.hpp>
#include <entt/signal/fwd.hpp>
#include <memory>
#include <unordered_map>

namespace pong {
class Game {
 public:
  struct Settings {
    int window_width;
    int window_height;
  };

  explicit Game(Settings settings) noexcept;
  ~Game();

  void Run() noexcept;

 private:
  static constexpr auto kTitle = "Pong";

  void Update(float delta_time) noexcept;
  void Render() noexcept;

  Settings settings_;

  core::scene::SceneManager scene_manager_;

  bool running_{};
};
}  // namespace pong
