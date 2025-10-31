#pragma once
#include <entt/entt.hpp>

namespace ECS {

    class TransformSystem {
        /*
        This class contains functions for updating the Model Matrix 
        */
    public:
      static void update(entt::registry& registry);
      static void updateModelMatrix(struct TransformComponent& transform);
    };

}
