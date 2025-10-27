#include "ecs/ecsManager.h"
#include "ecs/components.h"
#include <fstream>
#include <iostream>
#include "json/json.hpp"

using json = nlohmann::json;

namespace ECS {

    entt::registry ECSManager::s_Registry;

    void ECSManager::init()
    {
        s_Registry.clear();
        std::cout << "ECS Manager initialized" << std::endl;
    }

    void ECSManager::shutdown()
    {
        s_Registry.clear();
    }

    entt::registry& ECSManager::getRegistry()
    {
        return s_Registry;
    }

    void ECSManager::loadEntitiesFromFile(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Could not open entity config file: " << filepath << std::endl;
            return;
        }
        
        json data = json::parse(file);
        file.close();
        
        if (!data.contains("entities")) {
            std::cout << "No entities found in config file" << std::endl;
            return;
        }
        
        for (auto& entityData : data["entities"]) {
            // Create empty entity
            auto entity = s_Registry.create();
            
            std::string name = entityData.value("name", "Unnamed Entity");
            s_Registry.emplace<NameComponent>(entity, name);
            
            if (!entityData.contains("components")) {
                std::cerr << "Entity '" << name << "' has no components!" << std::endl;
                continue;
            }
            
            auto& components = entityData["components"];
            
            // ADD TRANSFORM COMPONENT (if specified)
            if (components.contains("transform")) {
                auto& transformData = components["transform"];
                
                glm::vec3 position(0.0f);
                glm::vec3 rotation(0.0f);
                float scale = 1.0f;
                
                if (transformData.contains("position")) {
                    position.x = transformData["position"][0];
                    position.y = transformData["position"][1];
                    position.z = transformData["position"][2];
                }
                
                if (transformData.contains("rotation")) {
                    rotation.x = transformData["rotation"][0];
                    rotation.y = transformData["rotation"][1];
                    rotation.z = transformData["rotation"][2];
                }
                
                if (transformData.contains("scale")) {
                    scale = transformData["scale"];
                }
                
                auto& transform = s_Registry.emplace<TransformComponent>(entity);
                transform.position = position;
                transform.rotation = rotation;
                transform.scale = scale;
            }
            
            // ADD MODEL COMPONENT
            if (components.contains("model")) {
                auto& modelData = components["model"];
                std::string modelPath = modelData.value("path", "");
                
                if (!modelPath.empty()) {
                    s_Registry.emplace<ModelComponent>(entity, modelPath);
                }
            }
            
            // ADD SHADER COMPONENT
            if (components.contains("shader")) {
                auto& shaderData = components["shader"];
                std::string vertPath = shaderData.value("vertex", "assets/shaders/default.vert");
                std::string fragPath = shaderData.value("fragment", "assets/shaders/default.frag");
                
                s_Registry.emplace<ShaderComponent>(entity, vertPath, fragPath);
            }
            
            
            // ADD TYPE
            if (components.contains("type")) {
                std::string typeName = components["type"]
                if (typeName == "renderStatic") {
                    s_Registry.emplace<RenderableTag>(entity);
                }
             }
            
            std::cout << "Loaded entity: " << name << std::endl;
        }
        
        std::cout << "Total entities loaded: " << s_Registry.size() << std::endl;
    }

    // SAVE ENTITIES BACK TO JSON
    void ECSManager::saveEntitiesToFile(const std::string& filepath)
    {
        std::ifstream fileRead(filepath);
        if (!fileRead.is_open()) {
            std::cerr << "Could not open entity config file for reading" << std::endl;
            return;
        }
        
        json data = json::parse(fileRead);
        fileRead.close();
        
        std::unordered_map<std::string, json*> entityMap;
        for (auto& entity : data["entities"]) {
            entityMap[entity["name"]] = &entity;
        }
        
        // Update transforms for all entities
        auto view = s_Registry.view<NameComponent, TransformComponent>();
        
        for (auto entity : view) {
            auto& name = view.get<NameComponent>(entity);
            auto& transform = view.get<TransformComponent>(entity);
            
            auto it = entityMap.find(name.name);
            if (it != entityMap.end()) {
                json& entityData = *(it->second);
                
                // Update transform component in JSON
                if (entityData["components"].contains("transform")) {
                    auto& transformData = entityData["components"]["transform"];
                    
                    transformData["position"][0] = transform.position.x;
                    transformData["position"][1] = transform.position.y;
                    transformData["position"][2] = transform.position.z;
                    
                    transformData["rotation"][0] = transform.rotation.x;
                    transformData["rotation"][1] = transform.rotation.y;
                    transformData["rotation"][2] = transform.rotation.z;
                    
                    transformData["scale"] = transform.scale;
                }
            }
        }
        
        std::ofstream fileWrite(filepath);
        fileWrite << data.dump(4);
        fileWrite.close();
        
        std::cout << "Saved entities to " << filepath << std::endl;
    }

    // HELPER: Create entity from code
    entt::entity ECSManager::createStaticEntity(
        const std::string& name,
        const std::string& modelPath,
        const glm::vec3& position,
        const glm::vec3& rotation,
        float scale)
    {
        auto entity = s_Registry.create();
        
        s_Registry.emplace<NameComponent>(entity, name);
        
        auto& transform = s_Registry.emplace<TransformComponent>(entity);
        transform.position = position;
        transform.rotation = rotation;
        transform.scale = scale;
        transform.needsUpdate = true;
        
        s_Registry.emplace<ModelComponent>(entity, modelPath);
        s_Registry.emplace<ShaderComponent>(entity, 
            "assets/shaders/default.vert", 
            "assets/shaders/default.frag");
        s_Registry.emplace<RenderableTag>(entity);
        s_Registry.emplace<StaticTag>(entity);
        
        return entity;
    }

}
