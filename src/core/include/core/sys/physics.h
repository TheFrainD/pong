#pragma once

#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>

namespace core::sys {

void PhysicsUpdate(entt::registry &reg, entt::dispatcher &dispatcher) noexcept;

}  // namespace core::sys