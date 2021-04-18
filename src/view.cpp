#include "view.hpp"

View::View(int W, int H, const int TILESIZE) : WIDTH{W}, HEIGHT{H}, default_tilesize{TILESIZE}, tilesize{TILESIZE}{}

void View::update(entt::registry & registry) {
    const auto view = registry.view<Player, Position>();
    for (const auto e : view) {
        Position &pos = view.get<Position>(e);

        //do this calculation in movement
        //pos.tileGX = floor(pos.globalX/default_tilesize);
        //pos.tileGY = floor(pos.globalY/default_tilesize);

        //Simple update when the tile size is the default size
        if(tilesize == default_tilesize){
            offsetX = (fmod(pos.globalX, static_cast<float>(tilesize)));
            offsetY = (fmod(pos.globalY, static_cast<float>(tilesize)));
            // zoom_offsetx = 0;
            // zoom_offsety = 0;
        }
        //If the tile size is not default, add "zoom offsets" to ensure the view remains centered
        else{
            offsetX = (fmod(pos.globalX, static_cast<float>(default_tilesize)) / static_cast<float>(default_tilesize)) * static_cast<float>(tilesize);
            offsetY = (fmod(pos.globalY, static_cast<float>(default_tilesize)) / static_cast<float>(default_tilesize)) * static_cast<float>(tilesize);
            // zoom_offsetx = (user.global[0] / static_cast<float>(default_tilesize)) - (user.global[0] / static_cast<float>(tilesize));
            // zoom_offsety = (user.global[1] / static_cast<float>(default_tilesize)) - (user.global[1] / static_cast<float>(tilesize));
        }

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

        xmod = (pos.tileGX < 0) ? offsetX + tilesize : offsetX;
        ymod = (pos.tileGY < 0) ? offsetY + tilesize : offsetY;

        pos.screenX = (-xmod + ((pos.tileGX - bounds[0][0]) * tilesize)) +
                    (static_cast<float>(pos.globalX%default_tilesize) /
                     static_cast<float>(default_tilesize))*tilesize;

        pos.screenY = (-ymod + ((pos.tileGY - bounds[0][1]) * tilesize)) +
                    (static_cast<float>(pos.globalY%default_tilesize) /
                    static_cast<float>(default_tilesize))*tilesize;
    }
}

//Give any entity currently within the view bounds the Rendered component
void View::updateEntities(entt::registry & registry, int & bound_count) {
    bound_count = 0;
    const auto entities = registry.view<Position>();

    for(const auto entity : entities) {
        Position &pos = entities.get<Position>(entity);

        //Remove the Rendered component from each entity
        //then add Rendered only if its position is within the view bounds
        if(registry.has<Rendered>(entity)) {
            registry.remove<Rendered>(entity);
        }

        //TODO
        //need a better solution
        //not all entities will need to have their tiles updated
        //pos.tileGX = floor(pos.globalX/default_tilesize);
        //pos.tileGY = floor(pos.globalY/default_tilesize);
        //TODO
        //Factor in entity width
        //Need Width component?
        if(pos.tileGX >= bounds[0][0] - 1 && pos.tileGX <= bounds[1][0] + 1) {
            if(pos.tileGY >= bounds[0][1] - 1 && pos.tileGY <= bounds[2][1] + 1) {
                pos.screenX = (-xmod + ((pos.tileGX - bounds[0][0]) * tilesize)) +
                            (static_cast<float>(pos.globalX%default_tilesize) /
                             static_cast<float>(default_tilesize))*tilesize;

                pos.screenY = (-ymod + ((pos.tileGY - bounds[0][1]) * tilesize)) +
                            (static_cast<float>(pos.globalY%default_tilesize) /
                            static_cast<float>(default_tilesize))*tilesize;

                registry.emplace<Rendered>(entity);
                bound_count++;
            }
        }
    }
}
