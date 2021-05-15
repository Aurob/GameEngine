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
        //printf("%f\n", WU.terrainGeneration(temp.tileGX, temp.tileGY));
        if(WU.terrainGeneration(temp.tileGX, temp.tileGY) >= .45) {
            position = temp;
            return true;

        }

    }
    return false;
}

void eMovement(Movement& movement, Position& entity, Position player) {
    vector<int> distdiff = { abs(player.tileGX - entity.tileGX), abs(player.tileGY - entity.tileGY) };

    switch(movement.type) {
        case cautious:
            //printf("...\n");
            if(distdiff[0] < 15 && distdiff[1] < 15) {
                movement.dx=player.globalX-entity.globalX;
                movement.dy=player.globalY-entity.globalY;
                //if(player.globalX-entity.globalX < 0) movement.dx=player.globalX-entity.globalX;
                //if(player.globalX-entity.globalX > 0) movement.dx=player.globalX-entity.globalX;
                //if(player.globalY-entity.globalY < 0) movement.dy=player.globalY-entity.globalY;
                //if(player.globalY-entity.globalY > 0) movement.dy=player.globalY-entity.globalY;
                entity.globalX+=(movement.dx<0)?-1:1;
                entity.globalY+=(movement.dy<0)?-1:1;
            }
            break;
        case aggresive:
            //printf("!!!\n");
            if(distdiff[0] < 5 && distdiff[1] < 5) {
                //Pathing & graph = registry.get<Pathing>(e);
                //vector<int> next_tile = graph.path({entity.tileGX, entity.tileGY}, {player.tileGX, player.tileGY});
                //if(next_tile[0]+player.tileGX > entity.tileGX) entity.globalX+=next_tile[0];//(rand()%3);
                //if(next_tile[0]+player.tileGX < entity.tileGX) entity.globalX-=next_tile[0];//(rand()%3);
                //if(next_tile[1]+player.tiwGY < entity.tileGY) entity.globalY-=next_tile[1];//(rand()%3);
                movement.dx=player.globalX-entity.globalX;
                movement.dy=player.globalY-entity.globalY;
                //if(player.globalX-entity.globalX < 0) movement.dx=player.globalX-entity.globalX;
                //if(player.globalX-entity.globalX > 0) movement.dx=player.globalX-entity.globalX;
                //if(player.globalY-entity.globalY < 0) movement.dy=player.globalY-entity.globalY;
                //if(player.globalY-entity.globalY > 0) movement.dy=player.globalY-entity.globalY;
                entity.globalX+=(movement.dx>0)?-1:1;
                entity.globalY+=(movement.dy>0)?-1:1;
            }
            break;
        case neutral:
            entity.globalX += sin(fmod(movement.timer,100) * 3.14159)*movement.speed;
            entity.globalY += cos(fmod(movement.timer,100) * 3.14159)*movement.speed;
            //printf("~~~\n");
            break;
    }
    entity.tileGX = floor(entity.globalX/64);
    entity.tileGY = floor(entity.globalY/64);

    movement.timer++;
    if(fmod(movement.timer,10)==0) {
        movement.step++;
        movement.step%=3;
    }
}

void entityMovement(entt::registry & registry, int tilesize, WorldUtils & WU) {
    Position usrpos;
    float n;
    int distance;
    auto player = registry.view<Player, Position>();
    for (const auto p : player) {
        usrpos = registry.get<Position>(p);
    }
    srand(time(NULL));
    auto view = registry.view<NPC, Position, Rendered, Pathing, Movement>();

    for (const auto e : view) {
        Position & pos = view.get<Position>(e);
        Pathing & graph = view.get<Pathing>(e);
        Movement & speed = view.get<Movement>(e);
        const Identification id = registry.get<Identification>(e);
        srand(id.ID*(rand()%60000));
        distance = 1;
        //printf("%f\n", WU.stepGeneration(usrpos.globalX + id.ID, usrpos.globalY + id.ID));
        /*if(graph.idle_time>0) {
            if(SDL_GetTicks()-graph.idle_time>3000) {
                graph.idle_time = 0;
            }
            else {
                pos.globalX += distance;
                pos.globalY += distance;
            }
        }
        else {*/
            vector<int> next_tile = {usrpos.tileGX-pos.tileGX, usrpos.tileGY-pos.tileGY};//graph.path({pos.tileGX, pos.tileGY}, {usrpos.tileGX, usrpos.tileGY});

            if(abs(next_tile[0]) < graph.draw_distance && abs(next_tile[1]) < graph.draw_distance) {
                if(next_tile[0] == 0 && next_tile[1] == 0) {
                    if(usrpos.globalX-pos.globalX == 0 || usrpos.globalY-pos.globalY == 0) {
                        pos.globalX += distance; //(rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                        pos.globalY += distance; //(rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                        graph.idle_time = SDL_GetTicks();
                    }
                    else {
                        if(usrpos.globalX-pos.globalX < 0) pos.globalX-=distance;
                        if(usrpos.globalX-pos.globalX > 0) pos.globalX+=distance;
                        if(usrpos.globalY-pos.globalY < 0) pos.globalY-=distance;
                        if(usrpos.globalY-pos.globalY > 0) pos.globalY+=distance;
                    }
                }
                if(next_tile[0]<0) pos.globalX-=distance;
                if(next_tile[0]>0) pos.globalX+=distance;
                if(next_tile[1]<0) pos.globalY-=distance;
                if(next_tile[1]>0) pos.globalY+=distance;
            }
            else {
                pos.globalX += speed.speed * speed.dx;
                pos.globalY += speed.speed * speed.dy;
                //pos.globalX += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                //pos.globalY += (rand()%2) * ((rand() % 2 < 1) ? -1 : 1);
                float radians = WU.stepGeneration(id.ID, id.ID);
                pos.globalX += sin(radians * 3.14159) * (speed.speed * speed.dx);
                pos.globalY += cos(radians * 3.14159) * (speed.speed * speed.dy);
            }
        //}
        //else
        //if(abs(pos.tileGX-userTileX) <= graph.draw_distance && abs(pos.tileGY-userTileY) <= graph.draw_distance){
            //graph.idle_time=0;


        pos.tileGX = floor(pos.globalX/64);
        pos.tileGY = floor(pos.globalY/64);
        //speed.time+=1000;
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
