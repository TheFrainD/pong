#include "sys/player_controller.h"

#include <raylib.h>

#include <entt/entity/registry.hpp>

#include "comp/paddle.h"
#include "comp/player.h"
#include "comp/sprite.h"
#include "comp/transform.h"

namespace pong::sys {

void PlayerControllerUpdate(entt::registry &reg, float delta_time,
                            int window_height) noexcept {
  auto view =
      reg.view<comp::Player, comp::Transform, comp::Sprite, comp::Paddle>();

  view.each([delta_time, window_height](const auto &player, auto &transform,
                                        const auto &sprite,
                                        const auto &paddle) {
    using comp::Paddle;
    const auto up_key = paddle.side == Paddle::Side::kLeft ? KEY_W : KEY_UP;
    const auto dowm_key = paddle.side == Paddle::Side::kLeft ? KEY_S : KEY_DOWN;

    if (IsKeyDown(up_key) && transform.position.y > 0.0F) {
      transform.position.y -= player.speed * delta_time;
    } else if (IsKeyDown(dowm_key) &&
               transform.position.y < (window_height - sprite.size.y)) {
      transform.position.y += player.speed * delta_time;
    }
  });
}

}  // namespace pong::sys