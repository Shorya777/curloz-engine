#include "ecs/systems/editorSystem.h"
#include "ecs/components.h"
#include "ecs/ecsManager.h"
#include "global/globalEntity.h"
#include "imgui/imgui.h"
#include "global/globalStatus.h"
#include <iostream>

namespace ECS {

    bool EditorSystem::s_ShowSaveButton = true;

    void EditorSystem::update(entt::registry& registry)
    {
        if (global::Status::Mode != global::MODE::EDIT_MODE) {
            return; // Only run in edit mode
        }

        ImGui::Begin("Entity Editor");
        ImGui::Separator();

        // Save button
        if (ImGui::Button("Save Entities")) {
            saveConfig();
        }

        ImGui::Separator();
        ImGui::Separator();

        // Show all entities with Transform component
        auto view = registry.view<NameComponent, TransformComponent>();

        int index = 0;
        for (auto entity : view) {
            auto& name = view.get<NameComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);

            ImGui::PushID(index++);

            if (ImGui::TreeNode(name.name.c_str())) {
                // Position slider
                glm::vec3 pos = transform.position;
                if (ImGui::SliderFloat3("Position", &pos.x, -50.0f, 50.0f)) {
                    transform.position = pos;
                    transform.needsUpdate = true;
                }

                // Rotation slider
                glm::vec3 rot = transform.rotation;
                if (ImGui::SliderFloat3("Rotation", &rot.x, -180.0f, 180.0f)) {
                    transform.rotation = rot;
                    transform.needsUpdate = true;
                }

                // Scale slider
                float scl = transform.scale;
                if (ImGui::SliderFloat("Scale", &scl, 0.1f, 10.0f)) {
                    transform.scale = scl;
                    transform.needsUpdate = true;
                }

                ImGui::TreePop();
            }

            ImGui::PopID();
            ImGui::Separator();
        }

        ImGui::End();
    }

    void EditorSystem::saveConfig()
    {
        ECSManager::saveEntitiesToFile(global::Entity::getConfigFile());
        std::cout << "Entities saved successfully!" << std::endl;
    }

}
