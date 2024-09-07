#include "sys/render.h"

#include <raylib.h>

#include <entt/entity/registry.hpp>

#include "comp/sprite.h"
#include "comp/transform.h"

namespace pong::sys {

void RenderSprites(entt::registry &reg) noexcept {
  auto view = reg.view<comp::Transform, comp::Sprite>();

  view.each([](const auto &transform, const auto &sprite) {
    DrawRectangleV(transform.position, sprite.size, sprite.color);
  });
}

}  // namespace pong::sys