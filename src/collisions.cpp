#include "collisions.hpp"

/*
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
*/

bool entityCollisions(const Position pos1, const Position pos2) {

    int Ax = pos1.globalX;
    int Ay = pos1.globalY;
    int Bx = pos2.globalX;
    int By = pos2.globalY;

    if(Ax < Bx && Ay < By) {
        if(Ax+pos1.size > Bx) {
            if(Ay+pos1.size > By) {
                //printf("Collision: entity bottom right, player top left\n");
                return true;
            }
        }
    }

    else if(Ax > Bx && Ay < By) {

        if(Ax < Bx + pos2.size ) {

            if(Ay + pos1.size > By) {
                //printf("Collision: entity bottom left, player top right\n\n\n");
                return true;
            }
        }
    }
    if(Ax > Bx && Ay > By) {
        if(Ax < Bx + pos2.size) {
            if(Ay < By + pos2.size) {
                //printf("Collision: entity top left, player bottom right\n");
                return true;
            }
        }
    }
    if(Ax < Bx && Ay > By) {
        if(Ax+pos1.size > Bx) {
            if(Ay < By+pos2.size) {
                //printf("Collision: entity top right, player top right\n");
                return true;
            }
        }
    }

    return false;
}
