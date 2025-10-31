#pragma once
#include <entt/entt.hpp>
#include <string>
#include <glm/glm.hpp>

namespace ECS {

    class ECSManager {
    public:
    /* 
    This class will contain all necessary functions for loading the entities 
    from the entity.json file, whenever you would want to add a new entity in
    the program just edit the entity.json file
    */
        static void init();
        static void shutdown();
        
        static entt::registry& getRegistry();
        
        // Entity creation helpers
        static entt::entity createStaticEntity(
            const std::string& name,
            const std::string& modelPath,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            float scale = 1.0f
        );
        
        static void loadEntitiesFromFile(const std::string& filepath);
        
        static void saveEntitiesToFile(const std::string& filepath);

    private:
        static entt::registry s_Registry;
    };

}
