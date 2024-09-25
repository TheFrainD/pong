#pragma once

#include <raylib.h>
#include <raymath.h>

#include <entt/entity/entity.hpp>

namespace core::comp {

struct Collider {
  Vector2 size;
  Vector2 offset = {0};
};

struct CollisionEvent {
  entt::entity a;
  entt::entity b;

  CollisionEvent(entt::entity a, entt::entity b) : a(a), b(b) {}
};

}  // namespace core::comp