#include "movement.hpp"
#include <stdio.h>


bool playerMovement(
    entt::registry & registry,
    std::map<SDL_Scancode, bool> & keys,
    int deltaTime,
    WorldUtils& WU) {

    auto view = registry.view<Player, Position, Movement>();
    for (const auto e : view) {
        float speed = view.get<Movement>(e).speed;
        Position & position = view.get<Position>(e);
        speed = ceil((static_cast<float>(deltaTime)/1000)*speed);

        Position temp = position;

        if(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
            temp.globalY-=speed;
        if(keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
            temp.globalX+=speed;
        if(keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
            temp.globalY+=speed;
        if(keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
            temp.globalX-=speed;

        temp.tileGX = floor(temp.globalX/64);
        temp.tileGY = floor(temp.globalY/64);
        if(WU.terrainGeneration(temp.tileGX, temp.tileGY) >= 0.45) {
            position = temp;
            return true;

        }

    }
    return false;
}

void entityMovement(entt::registry & registry, int tilesize, WorldUtils & WU) {
    Position usrpos;
    float n;
    auto player = registry.view<Player, Position>();
    for (const auto p : player) {
        usrpos = registry.get<Position>(p);
    }
    srand(time(NULL));
    auto view = registry.view<NPC, Position, Rendered, Pathing>();
    for (const auto e : view) {
        Position & pos = view.get<Position>(e);
        Pathing & graph = view.get<Pathing>(e);
        const Identification id = registry.get<Identification>(e);
        //Need a consistent srand so position updates
        // aren't affected by tile randomness

        if(graph.idle_time>0) {
            if(SDL_GetTicks()-graph.idle_time>3000) {
                graph.idle_time = 0;
            }
            else {
                pos.globalX += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                pos.globalY += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
            }
        }
        else {
            vector<int> next_tile = {usrpos.tileGX-pos.tileGX, usrpos.tileGY-pos.tileGY};//graph.path({pos.tileGX, pos.tileGY}, {usrpos.tileGX, usrpos.tileGY});

            if(abs(next_tile[0]) < graph.draw_distance && abs(next_tile[1]) < graph.draw_distance) {
                //printf("%d %d\n", usrpos.globalX-pos.globalX, usrpos.globalY-pos.globalY);
                if(next_tile[0] == 0 && next_tile[1] == 0) {
                    if(usrpos.globalX-pos.globalX == 0 || usrpos.globalY-pos.globalY == 0) {
                        pos.globalX += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                        pos.globalY += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                        graph.idle_time = SDL_GetTicks();
                    }
                    else {
                        if(usrpos.globalX-pos.globalX < 0) pos.globalX-=1;
                        if(usrpos.globalX-pos.globalX > 0) pos.globalX+=1;
                        if(usrpos.globalY-pos.globalY < 0) pos.globalY-=1;
                        if(usrpos.globalY-pos.globalY > 0) pos.globalY+=1;
                    }
                }
                if(next_tile[0]<0) pos.globalX-=1+(rand()%3);
                if(next_tile[0]>0) pos.globalX+=1+(rand()%3);
                if(next_tile[1]<0) pos.globalY-=1+(rand()%3);
                if(next_tile[1]>0) pos.globalY+=1+(rand()%3);
            }
            else {
                pos.globalX += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                pos.globalY += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
            }
        }
        //else
        //if(abs(pos.tileGX-userTileX) <= graph.draw_distance && abs(pos.tileGY-userTileY) <= graph.draw_distance){
            //graph.idle_time=0;


        pos.tileGX = floor(pos.globalX/64);
        pos.tileGY = floor(pos.globalY/64);
    }
}

//if(next_tile[0] != -1) {

                //if(next_tile[0]+usrpos.tileGX > pos.tileGX) pos.globalX+=next_tile[0];//(rand()%3);
                //if(next_tile[0]+usrpos.tileGX < pos.tileGX) pos.globalX-=next_tile[0];//(rand()%3);
                //if(next_tile[1]+usrpos.tileGY > pos.tileGY) pos.globalY+=next_tile[1];//(rand()%3);
                //if(next_tile[1]+usrpos.tileGY < pos.tileGY) pos.globalY-=next_tile[1];//(rand()%3);
                /*if(next_tile[0]+usrpos.tileGX==pos.tileGX && next_tile[1]+usrpos.tileGY==pos.tileGY) {
                    printf("SAME!");
                    if(usrpos.globalX >= pos.globalX) pos.globalX+=1;
                    if(usrpos.globalX <= pos.globalX) pos.globalX-=1;
                    if(usrpos.globalY >= pos.globalY) pos.globalY+=1;
                    if(usrpos.globalY <= pos.globalY) pos.globalY-=1;
                //    pos.globalX+=((rand()%2<1)?-1:1);
                //    pos.globalY+=((rand()%2<1)?-1:1);
                //    graph.idle_time = SDL_GetTicks();
                }*/
            //}
            //else {
            //    graph.idle_time = SDL_GetTicks();
           //     pos.globalX += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
            //    pos.globalY += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
            //}
        //}
        //else {
        //    pos.globalX+=((n*2<1)?-1:1);
        //    pos.globalY+=((n*2<1)?-1:1);
        //}
