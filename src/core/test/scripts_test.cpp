#include <core/sys/scripts.h>
#include <gtest/gtest.h>
#include <raylib.h>

#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>
#include <sol/object.hpp>

#include "core/comp/name.h"
#include "core/comp/script.h"
#include "core/comp/transform.h"

class ScriptTest : public testing::Test {
 protected:
  ScriptTest()
      : script_system_(
            registry_, dispatcher_,
            [this](const std::filesystem::path &) { return script_; }) {}

  entt::registry registry_;
  entt::dispatcher dispatcher_;

  core::sys::ScriptSystem script_system_;

  // Script mock
  std::string script_;
};

TEST_F(ScriptTest, Basic) {
  script_ = "test = 42";

  auto id = script_system_.RegisterScript("", {});
  auto script_entry = script_system_.GetScript(id);

  EXPECT_EQ(script_entry.env["test"].get<int>(), 42);
}

TEST_F(ScriptTest, GetComponentWorks) {
  script_ =
      "test = false\n"
      "function OnStart()\n"
      "  if self.GetComponent('Transform') then\n"
      "    test = true\n"
      "  end\n"
      "end\n";

  auto e = registry_.create();
  registry_.emplace<core::comp::Transform>(e, Vector2{10.0F, 10.0F});
  auto script_component =
      core::comp::AddScript(registry_, script_system_, e, "");
  ASSERT_TRUE(script_component.has_value());

  script_system_.OnStart();

  auto script_entry = script_system_.GetScript(script_component->scripts[""]);
  EXPECT_TRUE(script_entry.env["test"].get<bool>());
}

TEST_F(ScriptTest, ComponentModified) {
  script_ =
      "function OnStart()\n"
      "  self.GetComponent('Transform').position.x = 42.0\n"
      "end\n";

  auto e = registry_.create();
  auto &transform =
      registry_.emplace<core::comp::Transform>(e, Vector2{10.0F, 10.0F});
  core::comp::AddScript(registry_, script_system_, e, "");

  script_system_.OnStart();

  EXPECT_EQ(transform.position.x, 42.0F);
}

TEST_F(ScriptTest, GetEntityWorks) {
  script_ =
      "test = false\n"
      "function OnStart()\n"
      "  if GetEntity('TestName') then\n"
      "    test = true\n"
      "  end\n"
      "end\n";

  auto e1 = registry_.create();
  auto script_component =
      core::comp::AddScript(registry_, script_system_, e1, "");
  ASSERT_TRUE(script_component.has_value());

  auto e2 = registry_.create();
  registry_.emplace<core::comp::Name>(e2, "TestName");

  script_system_.OnStart();

  auto script_entry = script_system_.GetScript(script_component->scripts[""]);
  EXPECT_TRUE(script_entry.env["test"].get<bool>());
}

TEST_F(ScriptTest, ParametesWork) {
  script_ = "";

  auto e = registry_.create();
  auto script_component = core::comp::AddScript(
      registry_, script_system_, e, "",
      {{"test", sol::make_object(script_system_.GetState(), 42)}});
  ASSERT_TRUE(script_component.has_value());

  script_system_.OnStart();

  auto script_entry = script_system_.GetScript(script_component->scripts[""]);
  EXPECT_EQ(script_entry.env["test"].get_or(0), 42);
}
