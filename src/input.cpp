#include "input.hpp"
#include <stdio.h>

void playerInput(std::map<SDL_Scancode, bool> keys) {
}

bool mouseCollision(int mouse[2], Position entity) {
    if(mouse[0] > entity.screenX && mouse[0] < entity.screenX + entity.sizeS) {
        if(mouse[1] > entity.screenY && mouse[1] < entity.screenY + entity.sizeS) {
            return true;
        }
    }
    return false;
}

int entityInteractions(int mouse[2], bool & mousedown, entt::registry & registry) {
    const auto entities = registry.view<Interaction, Position, Rendered, Identification>();
    int count = 0;
    for(const auto e : entities) {
        Position p = entities.get<Position>(e);
        Identification i = entities.get<Identification>(e);
        count++;
        if(mouseCollision(mouse, p) && mousedown) {
            if(registry.has<Health>(e)) {
                Health & amt = registry.get<Health>(e);
                if(registry.has<Rock>(e)) {
                    amt.health-=10;
                    continue;
                }
                if(registry.has<NPC>(e)) {
                    if(amt.living) {
                        amt.health--;
                        if(amt.health <= 0) {
                            amt.living = false;
                            amt.timer = SDL_GetTicks();
                            mousedown = false;
                        }
                    }
                }
            }
        }
    }

    return count;
}
