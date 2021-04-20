#include "input.hpp"
#include <stdio.h>

void playerInput(std::map<SDL_Scancode, bool> keys) {
}

bool mouseCollision(int mouse[2], Position entity, int size) {
    if(mouse[0] > entity.screenX && mouse[0] < entity.screenX + size) {
        if(mouse[1] > entity.screenY && mouse[1] < entity.screenY + size) {
            return true;
        }
    }
    return false;
}

int entityInteractions(int mouse[2], bool mousedown, entt::registry & registry) {
    const auto entities = registry.view<Interaction, Position, Rendered, Identification>();
    int count = 0;
    for(const auto e : entities) {
        Position p = entities.get<Position>(e);
        Identification i = entities.get<Identification>(e);
        count++;
        if(mouseCollision(mouse, p, i.size) && mousedown) {
            if(registry.has<Health>(e)) {
                Health & amt = registry.get<Health>(e);
                if(--amt.health <= 0) {
                    registry.destroy(e);
                    count--;
                }
            }
        }
    }

    return count;
}
