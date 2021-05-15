#include "view.hpp"

View::View(int W, int H, const int TILESIZE) : WIDTH{W}, HEIGHT{H}, default_tilesize{TILESIZE}, tilesize{TILESIZE}{}

void View::update(entt::registry & registry) {
    const auto view = registry.view<Player, Position>();
    for (const auto e : view) {
        Position &pos = view.get<Position>(e);

        //Simple update when the tile size is the default size
        if(tilesize == default_tilesize){
            offsetX = (fmod(pos.globalX, static_cast<float>(tilesize)));
            offsetY = (fmod(pos.globalY, static_cast<float>(tilesize)));
        }
        //If the tile size is not default, add "zoom offsets" to ensure the view remains centered
        else{
            offsetX = (fmod(pos.globalX, static_cast<float>(default_tilesize)) / static_cast<float>(default_tilesize)) * static_cast<float>(tilesize);
            offsetY = (fmod(pos.globalY, static_cast<float>(default_tilesize)) / static_cast<float>(default_tilesize)) * static_cast<float>(tilesize);
        }

        //Fix for stuttering effect in negative coordinates
        if(pos.tileGX < 1 && pos.globalX < 0) pos.tileGX -= 1;
        if(pos.tileGY < 1 && pos.globalY < 0) pos.tileGY -= 1;

        //Set view bounds based on the user's global position
        //These values are used for determining which tiles to render
        bounds[0][0] = (pos.tileGX - ((WIDTH/tilesize)/2));
        bounds[0][1] = (pos.tileGY - ((HEIGHT/tilesize)/2));
        bounds[1][0] = bounds[0][0] + (WIDTH / tilesize);
        bounds[1][1] = bounds[0][1];
        bounds[2][0] = bounds[0][0];
        bounds[2][1] = bounds[0][1] + (HEIGHT / tilesize);
        bounds[3][0] = bounds[1][0];
        bounds[3][1] = bounds[2][1];

        //offset directions based on negative/positive positions
        xmod = (pos.tileGX < 0) ? offsetX + tilesize : offsetX;
        ymod = (pos.tileGY < 0) ? offsetY + tilesize : offsetY;

        //Zoom scaled size
        pos.size = tilesize/10;
    }
}

//Give any entity currently within the view bounds the Rendered component
void View::updateEntities(entt::registry & registry, int & bound_count) {
    bound_count = 0;
    const auto entities = registry.view<Position,Interaction>();
    const auto pView = registry.view<Player, Position, Health>();
    entt::entity player;
    for(auto p : pView) {
        player = p;
    }
    for(const auto entity : entities) {
        bound_count++;
        //Remove the Rendered component from each entity
        //then add Rendered only if its position is within the view bounds
        if(registry.has<Rendered>(entity)) {
            registry.remove<Rendered>(entity);
        }

        Position &pos = entities.get<Position>(entity);
        if(pos.tileGX >= bounds[0][0] - 1 && pos.tileGX <= bounds[1][0] + 1) {
            if(pos.tileGY >= bounds[0][1] - 1 && pos.tileGY <= bounds[2][1] + 1) {

                pos.screenX = (-xmod + ((pos.tileGX - bounds[0][0]) * tilesize)) +
                            (static_cast<float>(pos.globalX%default_tilesize) /
                             static_cast<float>(default_tilesize))*tilesize;

                pos.screenY = (-ymod + ((pos.tileGY - bounds[0][1]) * tilesize)) +
                            (static_cast<float>(pos.globalY%default_tilesize) /
                            static_cast<float>(default_tilesize))*tilesize;
                pos.sizeS = tilesize * (static_cast<float>(pos.size)/default_tilesize);

                if(registry.has<Movement>(entity)) {
                    Movement &mvmnt = registry.get<Movement>(entity);
                    Position usrpos = registry.get<Position>(player);
                    eMovement(mvmnt, pos, usrpos);
                }

                //Get the entities interaction component
                const Interaction iaction = entities.get<Interaction>(entity);
                //If interaction is damaging,
                //  check for collisions between this entity and the player
                if(iaction.damageOnCollision) {
                    Position &usrpos = registry.get<Position>(player);
                    if(entityCollisions(pos, usrpos)) {
                        Health &usrHP = registry.get<Health>(player);
                        usrHP.health-=1;
                    }
                }

                registry.emplace<Rendered>(entity);
            }
        }
    }

    /*
    const auto rendered = registry.view<Rendered>();
    for(const auto ent1 : rendered) {
        auto &p1 = registry.get<Position>(ent1);
        for(const auto ent2 : rendered) {
            if(ent1 == ent2) continue;
            auto &p2 = registry.get<Position>(ent2);
            if(entityCollisions(p1, p2)) {
                p2.direction *= -1;
            }
        }
    }
    */

}
