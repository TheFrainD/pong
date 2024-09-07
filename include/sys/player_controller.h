#pragma once

#include <entt/entity/fwd.hpp>

namespace pong::sys {

void PlayerControllerUpdate(entt::registry &reg, float delta_time,
                            int window_height) noexcept;

}