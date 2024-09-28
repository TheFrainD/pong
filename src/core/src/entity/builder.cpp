#include "core/entity/builder.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <utility>

#include "core/comp/name.h"
#include "core/comp/script/builder.h"
#include "core/comp/script/script.h"

namespace core::entity {

EntityBuilder::ScriptBuilderWrapper::ScriptBuilderWrapper(
    comp::ScriptBuilder script_builder, EntityBuilder &entity_builder,
    sys::ScriptSystem &script_system, entt::registry &registry,
    entt::entity &entity) noexcept
    : script_builder_(std::move(script_builder)),
      entity_builder_(entity_builder),
      script_system_(script_system),
      registry_(registry),
      entity_(entity) {}

EntityBuilder &EntityBuilder::ScriptBuilderWrapper::RegisterScript() noexcept {
  auto entry = script_builder_.GetEntry();
  const auto name = entry.name;
  const auto id = script_system_.RegisterScript(std::move(entry));
  if (id == -1) {
    return entity_builder_;
  }

  auto &comp = registry_.get_or_emplace<comp::ScriptComponent>(entity_);
  comp.scripts[name] = id;

  spdlog::debug("Registered '{}' script", name);

  return entity_builder_;
}

EntityBuilder::EntityBuilder(const std::string &name, entt::registry &registry,
                             sys::ScriptSystem &script_system) noexcept
    : entity_(registry.create()),
      registry_(registry),
      script_system_(script_system) {
  AddComponent<comp::Name>(name);
}

EntityBuilder::ScriptBuilderWrapper EntityBuilder::CreateScript(
    const std::filesystem::path &path) noexcept {
  return {comp::ScriptBuilder::Create(path, script_system_.GetState()).value(),
          *this, script_system_, registry_, entity_};
}

EntityBuilder::ScriptBuilderWrapper EntityBuilder::CreateScript(
    const std::string &script, const std::string &name) noexcept {
  return {comp::ScriptBuilder::Create(name, script, script_system_.GetState())
              .value(),
          *this, script_system_, registry_, entity_};
}

entt::entity &&EntityBuilder::GetEntity() noexcept {
  return std::move(entity_);
}

EntityBuilder &EntityBuilder::New(const std::string &name) noexcept {
  entity_ = registry_.create();
  AddComponent<comp::Name>(name);
  return *this;
}

}  // namespace core::entity
