#include "ecs/systems/transformSystem.h"
#include "ecs/components.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ECS{
    void TransformSystem::update(entt::registry& registry)
    {
        auto view = registry.view<TransformComponent>();
        
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            
            if (transform.needsUpdate) {
                updateModelMatrix(transform);
                transform.needsUpdate = false;
            }
        }
    }

    void TransformSystem::updateModelMatrix(TransformComponent& transform)
    {
        transform.modelMatrix = glm::mat4(1.0f);
        
        transform.modelMatrix = glm::translate(transform.modelMatrix, transform.position);
        
        transform.modelMatrix = glm::rotate(transform.modelMatrix, 
            glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform.modelMatrix = glm::rotate(transform.modelMatrix, 
            glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform.modelMatrix = glm::rotate(transform.modelMatrix, 
            glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        
        transform.modelMatrix = glm::scale(transform.modelMatrix, glm::vec3(transform.scale));

    }
}
