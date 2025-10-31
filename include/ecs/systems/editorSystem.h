#pragma once
#include <entt/entt.hpp>

namespace ECS {

    class EditorSystem {
        /*
        This class is for managing the ImGUI edit window during debug mode
        */
        public:
            static void update(entt::registry& registry);
            static void saveConfig();

        private:
            static bool s_ShowSaveButton;
    };

}
