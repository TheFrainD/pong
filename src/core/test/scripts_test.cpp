#include <core/sys/scripts.h>
#include <gtest/gtest.h>
#include <raylib.h>

#include <entt/entity/fwd.hpp>
#include <entt/signal/fwd.hpp>
#include <sol/object.hpp>

#include "core/comp/script/script.h"
#include "core/comp/transform.h"
#include "core/entity/builder.h"
#include "core/scene/scene_manager.h"

namespace {}

class ScriptTest : public testing::Test {
 protected:
  ScriptTest() : script_system_(registry_, dispatcher_, scene_manager_) {}

  entt::registry registry_;
  entt::dispatcher dispatcher_;

  core::scene::SceneManager scene_manager_;
  core::sys::ScriptSystem script_system_;
};

using core::entity::EntityBuilder;

TEST_F(ScriptTest, GetComponentWorks) {
  std::string script =
      "test = false\n"
      "function OnStart()\n"
      "  if self.GetComponent('Transform') then\n"
      "    test = true\n"
      "  end\n"
      "end\n";

  auto e = EntityBuilder("", registry_, script_system_)
               .AddComponent<core::comp::Transform>(Vector2{10.0F, 10.0F})
               .CreateScript(script, "")
               .RegisterScript()
               .GetEntity();

  script_system_.OnStart();

  auto script_component = registry_.get<core::comp::ScriptComponent>(e);
  auto script_entry = script_system_.GetScript(script_component.scripts[""]);
  EXPECT_TRUE(script_entry.env["test"].get<bool>());
}

TEST_F(ScriptTest, ComponentModified) {
  std::string script =
      "function OnStart()\n"
      "  self.GetComponent('Transform').position.x = 42.0\n"
      "end\n";

  auto e = EntityBuilder("", registry_, script_system_)
               .AddComponent<core::comp::Transform>(Vector2{10.0F, 10.0F})
               .CreateScript(script, "")
               .RegisterScript()
               .GetEntity();

  script_system_.OnStart();

  auto transform = registry_.get<core::comp::Transform>(e);
  EXPECT_EQ(transform.position.x, 42.0F);
}

TEST_F(ScriptTest, GetEntityWorks) {
  std::string script =
      "test = false\n"
      "function OnStart()\n"
      "  if GetEntity('TestName') then\n"
      "    test = true\n"
      "  end\n"
      "end\n";

  auto entity_builder = EntityBuilder("", registry_, script_system_);

  auto e1 =
      entity_builder.CreateScript(script, "").RegisterScript().GetEntity();
  auto e2 = entity_builder.New("TestName").GetEntity();

  script_system_.OnStart();

  auto script_component = registry_.get<core::comp::ScriptComponent>(e1);
  auto script_entry = script_system_.GetScript(script_component.scripts[""]);
  EXPECT_TRUE(script_entry.env["test"].get<bool>());
}

TEST_F(ScriptTest, ParametesWork) {
  auto e = EntityBuilder("", registry_, script_system_)
               .CreateScript("", "")
               .AddParameter("test", 42)
               .RegisterScript()
               .GetEntity();

  script_system_.OnStart();

  auto script_component = registry_.get<core::comp::ScriptComponent>(e);
  auto script_entry = script_system_.GetScript(script_component.scripts[""]);
  EXPECT_EQ(script_entry.env["test"].get_or(0), 42);
}
