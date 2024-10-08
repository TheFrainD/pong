#pragma once

#include <core/sys/scripts.h>

#include <entt/entt.hpp>

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

  entt::registry &GetRegistry() noexcept;

 private:
  static constexpr auto kTitle = "Pong";

  void Update(float delta_time) noexcept;
  void Render() noexcept;

  Settings settings_;
  entt::registry registry_;
  entt::dispatcher dispatcher_;

  core::sys::ScriptSystem script_system_;
};
}  // namespace pong
