#include "ecs/systems/renderSystem.h"
#include "ecs/components.h"
#include "global/globalCamera.h"
#include "global/globalDisplay.h"
#include "lighting/lighting.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ECS {

    glm::mat4 RenderSystem::s_ProjectionMatrix;
    glm::mat4 RenderSystem::s_ViewMatrix;

    void RenderSystem::init()
    {
        s_ProjectionMatrix = glm::perspective(
                glm::radians(global::CameraManager::camera->fov),
                (float)global::Display::getWidth() / (float)global::Display::getheight(),
                global::CameraManager::m_Near,
                global::CameraManager::m_Far
                );
        s_ViewMatrix = global::CameraManager::camera->GetViewMatrix();
    }


    void RenderSystem::render(entt::registry& registry, float deltaTime)
    {
        s_ViewMatrix = global::CameraManager::camera->GetViewMatrix();

        auto view = registry.view<TransformComponent, ModelComponent, ShaderComponent, RenderableTag>();

        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& m_model = view.get<ModelComponent>(entity);
            auto& m_shader = view.get<ShaderComponent>(entity);

            m_shader.shader.use();

            m_shader.shader.setMat4("projection", s_ProjectionMatrix);
            m_shader.shader.setMat4("view", s_ViewMatrix);
            m_shader.shader.setMat4("model", transform.modelMatrix);

            m_shader.shader.setVec3("directionalLight.direction", 
                    Lighting::getGlobalLight_Direction());
            m_shader.shader.setVec3("directionalLight.ambient", 
                    Lighting::getGlobalLight_Color()[0]);
            m_shader.shader.setVec3("directionalLight.diffuse", 
                    Lighting::getGlobalLight_Color()[1]);
            m_shader.shader.setVec3("directionalLight.specular", 
                    Lighting::getGlobalLight_Color()[2]);

            m_shader.shader.setInt("numPointLights", Lighting::getNumPointLights());
            for(int i = 0; i < Lighting::getNumPointLights(); ++i) {
                std::string base = "pointLight[" + std::to_string(i) + "].";
                m_shader.shader.setVec3(base + "position", 
                        Lighting::m_point_lights[i]->getPosition());
                m_shader.shader.setVec3(base + "ambient", 
                        Lighting::m_point_lights[i]->getColor()[0]);
                m_shader.shader.setVec3(base + "diffuse", 
                        Lighting::m_point_lights[i]->getColor()[1]);
                m_shader.shader.setVec3(base + "specular", 
                        Lighting::m_point_lights[i]->getColor()[2]);
            }

            m_shader.shader.setMat3("normalCalculationMatrix", 
                    glm::mat3(glm::transpose(glm::inverse(transform.modelMatrix))));
            m_shader.shader.setVec3("cameraPosition", 
                    global::CameraManager::camera->getPosition());

            m_model.model.Draw(m_shader.shader);
        }
    }

}
