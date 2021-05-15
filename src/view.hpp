#pragma once
#include "entt.hpp"
#include "components.hpp"
#include "collisions.hpp"
#include "movement.hpp"
#include <math.h>
#include <string>
#include "FastNoise.h"

class View {
    public:
        View(int, int, int);
        void update(entt::registry &);
        void updateEntities(entt::registry &, int&);

        int WIDTH, HEIGHT;
        int default_tilesize;
        int tilesize;
        int bounds[4][2];
        int offsetX;
        int offsetY;
        int xmod, ymod;
        //FastNoise noise;
};
