#include "sys/render.h"

#include <raylib.h>

#include <entt/entity/registry.hpp>

#include "comp/label.h"
#include "comp/sprite.h"
#include "comp/transform.h"

namespace pong::sys {

namespace {
void RenderSprites(entt::registry &reg) noexcept {
  auto view = reg.view<comp::Transform, comp::Sprite>();

  view.each([](const auto &transform, const auto &sprite) {
    DrawRectangleV(transform.position, sprite.size, sprite.color);
  });
}

void RenderText(entt::registry &reg) noexcept {
  auto view = reg.view<comp::Transform, comp::Label>();

  view.each([](const auto &transform, const auto &label) {
    DrawText(label.text.c_str(), transform.position.x, transform.position.y,
             label.font_size, label.color);
  });
}
}  // namespace

void Render(entt::registry &reg) noexcept {
  RenderSprites(reg);
  RenderText(reg);
}

}  // namespace pong::sys