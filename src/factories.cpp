#include "factories.hpp"

//Creates an NPC entity at a specified global position
void makeNPC(entt::registry & registry, int globalX, int globalY, int default_tilesize) {
    const auto npc = registry.create();
    registry.emplace<NPC>(npc);

    Position position {
        globalX, globalY,
        static_cast<int>(globalX/default_tilesize),
        static_cast<int>(globalY/default_tilesize),
        .size=25 + rand() % default_tilesize,
        .direction = ((rand()%2==0)?-1:1)
    };
    registry.emplace<Position>(npc, position);

    registry.emplace<Health>(npc, Health{100,100,true});
    registry.emplace<Interaction>(npc, Interaction{true});
    registry.emplace<Identification>(npc, Identification{rand()%100000, 0, 0, 0});
    registry.emplace<Movement>(npc, Movement{
                               1, 0,
                               ((rand()%2==0)?-1:1), ((rand()%2==0)?-1:1),
                               static_cast<Behaviour>(rand() % 3)
                               });
    //Pathing p(5);
    //registry.emplace<Pathing>(npc, p);

    registry.emplace<Textured>(npc, Textured{"chars", 0});
}

void makeRock(entt::registry & registry, int globalX, int globalY, int r, int g, int b, int size) {
    const auto rock = registry.create();
    registry.emplace<Rock>(rock);
    Position position {
        globalX, globalY,
        static_cast<int>(globalX/64),
        static_cast<int>(globalY/64),
        .size=size
    };
    registry.emplace<Position>(rock, position);
    registry.emplace<Health>(rock, Health{100,100});
    registry.emplace<Interaction>(rock, Interaction{false});
    registry.emplace<Identification>(rock, Identification{rand()%100000, r, g, b});
    registry.emplace<Textured>(rock, Textured{"misc", rand()%90});
}
