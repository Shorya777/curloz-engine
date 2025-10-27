#pragma once
#include <entt/entt.hpp>

namespace ECS {

    class EditorSystem {
        public:
            static void update(entt::registry& registry);
            static void saveConfig();

        private:
            static bool s_ShowSaveButton;
    };

}
