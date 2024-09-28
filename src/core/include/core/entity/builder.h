#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>

#include "core/comp/script/builder.h"
#include "core/sys/scripts.h"

namespace core::entity {

class EntityBuilder {
 public:
  class ScriptBuilderWrapper {
   public:
    ScriptBuilderWrapper(comp::ScriptBuilder script_builder,
                         EntityBuilder &entity_builder,
                         sys::ScriptSystem &script_system,
                         entt::registry &registry,
                         entt::entity &entity) noexcept;

    template <typename T, typename... Args>
    ScriptBuilderWrapper &AddParameter(const std::string &name,
                                       Args &&...args) noexcept {
      script_builder_.AddParameter<T>(name, std::forward<Args>(args)...);

      return *this;
    }

    template <typename T>
    ScriptBuilderWrapper &AddParameter(const std::string &name,
                                       const T &value) noexcept {
      script_builder_.AddParameter(name, value);

      return *this;
    }

    EntityBuilder &RegisterScript() noexcept;

   private:
    comp::ScriptBuilder script_builder_;
    EntityBuilder &entity_builder_;
    sys::ScriptSystem &script_system_;
    entt::registry &registry_;
    entt::entity &entity_;
  };

  EntityBuilder(const std::string &name, entt::registry &registry,
                sys::ScriptSystem &script_system) noexcept;

  ScriptBuilderWrapper CreateScript(const std::filesystem::path &path) noexcept;

  ScriptBuilderWrapper CreateScript(const std::string &script,
                                    const std::string &name) noexcept;

  entt::entity &&GetEntity() noexcept;

  EntityBuilder &New(const std::string &name) noexcept;

  template <typename T, typename... Args>
  EntityBuilder &AddComponent(Args &&...args) noexcept {
    registry_.emplace_or_replace<T>(entity_, std::forward<Args>(args)...);
    return *this;
  }

  template <typename T>
  EntityBuilder &AddComponent(const T &component) noexcept {
    registry_.emplace_or_replace<T>(entity_, component);
    return *this;
  }

 private:
  entt::entity entity_;
  entt::registry &registry_;
  sys::ScriptSystem &script_system_;
};

}  // namespace core::entity
