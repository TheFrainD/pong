#include <core/comp/collider.h>
#include <core/comp/transform.h>
#include <core/sys/physics.h>
#include <gtest/gtest.h>

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <vector>

class CollisionTest : public testing::Test {
 protected:
  CollisionTest() {
    dispatcher.sink<core::comp::CollisionEvent>()
        .connect<&CollisionTest::CollisionEvent>(this);
  }

  void CollisionEvent(const core::comp::CollisionEvent &event) {
    events.push_back(event);
  }

  bool HasCollision(entt::entity a, entt::entity b) {
    for (const auto &event : events) {
      if ((event.a == a && event.b == b) || (event.a == b && event.b == a)) {
        return true;
      }
    }
    return false;
  }

  std::vector<core::comp::CollisionEvent> events;

  entt::registry registry;
  entt::dispatcher dispatcher;
};

TEST_F(CollisionTest, Collision) {
  auto e1 = registry.create();
  registry.emplace<core::comp::Transform>(e1, Vector2{2.0f, 1.0f});
  registry.emplace<core::comp::Collider>(e1, Vector2{3.0f, 2.0f});

  auto e2 = registry.create();
  registry.emplace<core::comp::Transform>(e2, Vector2{4.0f, 2.0f});
  registry.emplace<core::comp::Collider>(e2, Vector2{2.0f, 2.0f});

  core::sys::PhysicsUpdate(registry, dispatcher);

  ASSERT_TRUE(HasCollision(e1, e2));
}

TEST_F(CollisionTest, NoCollision) {
  auto e1 = registry.create();
  registry.emplace<core::comp::Transform>(e1, Vector2{2.0f, 1.0f});
  registry.emplace<core::comp::Collider>(e1, Vector2{3.0f, 2.0f});

  auto e2 = registry.create();
  registry.emplace<core::comp::Transform>(e2, Vector2{5.0f, 2.0f});
  registry.emplace<core::comp::Collider>(e2, Vector2{2.0f, 2.0f});

  core::sys::PhysicsUpdate(registry, dispatcher);

  ASSERT_FALSE(HasCollision(e1, e2));
}

TEST_F(CollisionTest, OffsetCollision) {
  auto e1 = registry.create();
  registry.emplace<core::comp::Transform>(e1, Vector2{2.0f, 1.0f});
  registry.emplace<core::comp::Collider>(e1, Vector2{3.0f, 2.0f});

  auto e2 = registry.create();
  registry.emplace<core::comp::Transform>(e2, Vector2{5.0f, 2.0f});
  registry.emplace<core::comp::Collider>(e2, Vector2{2.0f, 2.0f},
                                         Vector2{-1.0f, 0.0f});

  core::sys::PhysicsUpdate(registry, dispatcher);

  ASSERT_TRUE(HasCollision(e1, e2));
}

TEST_F(CollisionTest, OffsetNoCollision) {
  auto e1 = registry.create();
  registry.emplace<core::comp::Transform>(e1, Vector2{2.0f, 1.0f});
  registry.emplace<core::comp::Collider>(e1, Vector2{3.0f, 2.0f});

  auto e2 = registry.create();
  registry.emplace<core::comp::Transform>(e2, Vector2{4.0f, 2.0f});
  registry.emplace<core::comp::Collider>(e2, Vector2{2.0f, 2.0f},
                                         Vector2{1.0f, 0.0f});

  core::sys::PhysicsUpdate(registry, dispatcher);

  ASSERT_FALSE(HasCollision(e1, e2));
}