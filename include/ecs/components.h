#pragma once
#include <glm/glm.hpp>
#include <string>
#include "loader/model.h"
#include "shader/shader.h"

namespace ECS {

    struct TransformComponent {
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f);
        float scale = 1.0f;
        
        glm::mat4 modelMatrix = glm::mat4(1.0f); 
        bool needsUpdate = true;  //flag to recalculate matrix
    };

    struct NameComponent {
        std::string name = "Unnamed Entity";
    };

    struct ModelComponent {
        std::string modelPath;
        Model model;
        
        ModelComponent() = default;
        ModelComponent(const std::string& path) 
            : modelPath(path), model(path) {}
    };

    struct ShaderComponent {
        Shader shader;
        
        ShaderComponent() = default;
        ShaderComponent(const std::string& vertPath, const std::string& fragPath)
            : shader(vertPath.c_str(), fragPath.c_str()) {}
    };

    struct RenderableType {};

    struct StaticTag {};

}
