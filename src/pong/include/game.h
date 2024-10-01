#pragma once

#include <core/scene/scene.h>
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

  std::shared_ptr<core::scene::Scene> GetCurrentScene();

 private:
  static constexpr auto kTitle = "Pong";

  void Update(float delta_time) noexcept;
  void Render() noexcept;

  entt::registry &GetRegistry();
  entt::dispatcher &GetDispatcher();

  Settings settings_;

  std::unordered_map<std::string, std::shared_ptr<core::scene::Scene>> scenes_;
  std::string current_scene_;
};
}  // namespace pong
