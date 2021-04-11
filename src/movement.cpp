#include "movement.hpp"
#include <stdio.h>

void playerMovement(entt::registry & registry, std::map<SDL_Scancode, bool> & keys, int deltaTime) {
    auto view = registry.view<Player, Position, Movement>();
    for (const auto e : view) {
        float speed = view.get<Movement>(e).speed;
        Position & position = view.get<Position>(e);
        speed = ceil((static_cast<float>(deltaTime)/1000)*speed);
        if(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
            position.globalY-=speed;
        if(keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
            position.globalX+=speed;
        if(keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
            position.globalY+=speed;
        if(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
            position.globalX-=speed;

        position.tileGX = floor(position.globalX/64);
        position.tileGY = floor(position.globalY/64);
    }
}

void entityMovement(entt::registry & registry, int tilesize) {
    int userTileX;
    int userTileY;
    auto player = registry.view<Player, Position>();
    for (const auto p : player) {
        Position pos = registry.get<Position>(p);
        userTileX = pos.tileGX;
        userTileY = pos.tileGY;
    }

    auto view = registry.view<NPC, Position, Rendered, Pathing>();
    for (const auto e : view) {
        Position & pos = view.get<Position>(e);
        Pathing & graph = view.get<Pathing>(e);
        const Identification id = registry.get<Identification>(e);
        //Need a consistent srand so position updates
        // aren't affected by tile randomness
        srand(id.ID);
        srand(rand()%10000);
        if(graph.idle_time>0 && SDL_GetTicks()-graph.idle_time<3) {
            pos.globalX+=((rand()%2<1)?-1:1);
            pos.globalY+=((rand()%2<1)?-1:1);
        }
        else if(abs(pos.tileGX-userTileX) <= graph.draw_distance && abs(pos.tileGY-userTileY) <= graph.draw_distance){
            graph.idle_time=0;
            vector<int> next_tile = graph.path({pos.tileGX, pos.tileGY}, {userTileX, userTileY});

            if(next_tile[0]+userTileX >= pos.tileGX+graph.draw_distance/2) pos.globalX+=1;
            if(next_tile[0]+userTileX <= pos.tileGX-graph.draw_distance/2) pos.globalX-=1;
            if(next_tile[1]+userTileY >= pos.tileGY+graph.draw_distance/2) pos.globalY+=1;
            if(next_tile[1]+userTileY <= pos.tileGY-graph.draw_distance/2) pos.globalY-=1;

            if(next_tile[0] != -1) {
                if(next_tile[0]+userTileX==pos.tileGX && next_tile[0]+userTileX==pos.tileGX) {
                    pos.globalX+=((rand()%2<1)?-1:1);
                    pos.globalY+=((rand()%2<1)?-1:1);
                    graph.idle_time = SDL_GetTicks();
                }
            }
            else {
                graph.idle_time = SDL_GetTicks();
                pos.globalX += (rand()%3) * ((rand() % 2 < 1) ? -1 : 1);
                pos.globalY += (rand()%3) * ((rand() % 2 < 1) ? -1 : 1);
            }
        }

        pos.tileGX = floor(pos.globalX/64);
        pos.tileGY = floor(pos.globalY/64);
    }
}
