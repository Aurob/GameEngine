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
    registry.emplace<Identification>(npc, Identification{rand()%100000, 0, 0, 0});

    Pathing p(5);
    registry.emplace<Pathing>(npc, p);
}

void makeRock(entt::registry & registry, int globalX, int globalY, int r, int g, int b) {
    const auto rock = registry.create();
    registry.emplace<Rock>(rock);
    Position position {
        globalX, globalY,
        static_cast<int>(globalX/64),
        static_cast<int>(globalY/64)
    };
    registry.emplace<Position>(rock, position);
    registry.emplace<Health>(rock, Health{100,100});
    registry.emplace<Interaction>(rock);
    registry.emplace<Identification>(rock, Identification{rand()%100000, r, g, b});
}
