#include "factories.hpp"

//Creates an NPC entity at a specified global position
void makeNPC(entt::registry & registry, int globalX, int globalY, int default_tilesize) {
    const auto npc = registry.create();
    registry.emplace<NPC>(npc);

    Position position {
        globalX, globalY,
        static_cast<int>(globalX/default_tilesize),
        static_cast<int>(globalY/default_tilesize)
    };
    registry.emplace<Position>(npc, position);

    registry.emplace<Health>(npc, Health{100,100});
    registry.emplace<Interaction>(npc);
    registry.emplace<Identification>(npc, Identification{rand()%100000});

    Pathing p(6);
    registry.emplace<Pathing>(npc, p);
}
