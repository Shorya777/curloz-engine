#pragma once
#include <entt/entt.hpp>

namespace ECS {

    class TransformSystem {
    public:
      static void update(entt::registry& registry);
      static void updateModelMatrix(struct TransformComponent& transform);
    };

}
