#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace ECS {

    class RenderSystem {
    public:
        static void init();
        static void render(entt::registry& registry, float deltaTime);
        
    private:
        static glm::mat4 s_ProjectionMatrix;
        static glm::mat4 s_ViewMatrix;
    };

}
