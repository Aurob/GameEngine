#include "collisions.hpp"

void entityInteractions(int mouse[2], bool mousedown, entt::registry & registry) {
    const auto entities = registry.view<NPC, Interaction, Position, Rendered>();
    for(const auto e : entities) {
        Position p = entities.get<Position>(e);
        if(mouseCollision(mouse, p) && mousedown) {
            if(registry.has<Health>(e)) {
                Health & amt = registry.get<Health>(e);
                if(--amt.health <= 0)
                    registry.destroy(e);
            }

        }
    }
}
