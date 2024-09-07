#include "sys/render.h"

#include <raylib.h>

#include <entt/entity/registry.hpp>

#include "comp/sprite_renderer.h"
#include "comp/transform.h"

namespace pong::sys {

void RenderSprites(entt::registry &reg) noexcept {
  auto view = reg.view<comp::Transform, comp::SpriteRenderer>();

  view.each([](const auto &transform, const auto &sprite) {
    DrawRectangleV(transform.position, sprite.size, sprite.color);
  });
}

}  // namespace pong::sys