#include "core/sys/physics.h"

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/comp/collider.h"
#include "core/comp/transform.h"

namespace core::sys {

void PhysicsUpdate(entt::registry &reg, entt::dispatcher &dispatcher) noexcept {
  using comp::Collider, comp::Transform, comp::CollisionEvent;

  auto view = reg.view<Transform, Collider>();

  view.each([&view, &dispatcher](entt::entity entity_a, Transform transform_a,
                                 Collider collider_a) {
    for (auto entity_b : view) {
      if (entity_a == entity_b) {
        continue;
      }

      Collider collider_b = view.get<Collider>(entity_b);
      Transform transform_b = view.get<Transform>(entity_b);

      Vector2 adjusted_position_a =
          Vector2Add(transform_a.position, collider_a.offset);
      Vector2 adjusted_position_b =
          Vector2Add(transform_b.position, collider_b.offset);

      bool collision =
          (adjusted_position_a.x < adjusted_position_b.x + collider_b.size.x &&
           adjusted_position_a.x + collider_a.size.x > adjusted_position_b.x &&
           adjusted_position_a.y < adjusted_position_b.y + collider_b.size.y &&
           adjusted_position_a.y + collider_a.size.y > adjusted_position_b.y);

      if (collision) {
        dispatcher.trigger(CollisionEvent{entity_a, entity_b});
      }
    }
  });
}

}  // namespace core::sys