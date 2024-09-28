#include <core/comp/collider.h>
#include <core/comp/transform.h>
#include <core/sys/physics.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <vector>

class CollisionTest : public testing::Test {
 protected:
  CollisionTest() {
    dispatcher_.sink<core::comp::CollisionEvent>()
        .connect<&CollisionTest::CollisionEvent>(this);
  }

  void CollisionEvent(const core::comp::CollisionEvent &event) {
    events_.push_back(event);
  }

  bool HasCollision(entt::entity a, entt::entity b) {
    return std::ranges::any_of(events_, [a, b](const auto &event) {
      return (event.a == a && event.b == b) || (event.a == b && event.b == a);
    });
  }

  std::vector<core::comp::CollisionEvent> events_;

  entt::registry registry_;
  entt::dispatcher dispatcher_;
};

TEST_F(CollisionTest, Collision) {
  auto e1 = registry_.create();
  registry_.emplace<core::comp::Transform>(e1, Vector2{2.0F, 1.0F});
  registry_.emplace<core::comp::Collider>(e1, Vector2{3.0F, 2.0F});

  auto e2 = registry_.create();
  registry_.emplace<core::comp::Transform>(e2, Vector2{4.0F, 2.0F});
  registry_.emplace<core::comp::Collider>(e2, Vector2{2.0F, 2.0F});

  core::sys::PhysicsUpdate(registry_, dispatcher_);

  EXPECT_TRUE(HasCollision(e1, e2));
}

TEST_F(CollisionTest, NoCollision) {
  auto e1 = registry_.create();
  registry_.emplace<core::comp::Transform>(e1, Vector2{2.0F, 1.0F});
  registry_.emplace<core::comp::Collider>(e1, Vector2{3.0F, 2.0F});

  auto e2 = registry_.create();
  registry_.emplace<core::comp::Transform>(e2, Vector2{5.0F, 2.0F});
  registry_.emplace<core::comp::Collider>(e2, Vector2{2.0F, 2.0F});

  core::sys::PhysicsUpdate(registry_, dispatcher_);

  EXPECT_FALSE(HasCollision(e1, e2));
}

TEST_F(CollisionTest, OffsetCollision) {
  auto e1 = registry_.create();
  registry_.emplace<core::comp::Transform>(e1, Vector2{2.0F, 1.0F});
  registry_.emplace<core::comp::Collider>(e1, Vector2{3.0F, 2.0F});

  auto e2 = registry_.create();
  registry_.emplace<core::comp::Transform>(e2, Vector2{5.0F, 2.0F});
  registry_.emplace<core::comp::Collider>(e2, Vector2{2.0F, 2.0F},
                                          Vector2{-1.0F, 0.0F});

  core::sys::PhysicsUpdate(registry_, dispatcher_);

  EXPECT_TRUE(HasCollision(e1, e2));
}

TEST_F(CollisionTest, OffsetNoCollision) {
  auto e1 = registry_.create();
  registry_.emplace<core::comp::Transform>(e1, Vector2{2.0F, 1.0F});
  registry_.emplace<core::comp::Collider>(e1, Vector2{3.0F, 2.0F});

  auto e2 = registry_.create();
  registry_.emplace<core::comp::Transform>(e2, Vector2{4.0F, 2.0F});
  registry_.emplace<core::comp::Collider>(e2, Vector2{2.0F, 2.0F},
                                          Vector2{1.0F, 0.0F});

  core::sys::PhysicsUpdate(registry_, dispatcher_);

  EXPECT_FALSE(HasCollision(e1, e2));
}
