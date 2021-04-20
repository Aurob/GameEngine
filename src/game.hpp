#pragma once
#include "render.hpp"
#include "view.hpp"
#include "movement.hpp"
#include "input.hpp"
#include "factories.hpp"
#include <map>
#include <SDL_timer.h>
#include <SDL_scancode.h>
#include "FastNoise.h"
#include "worldgen.hpp"
class Game {
    public:
        Game(int, int, int);
        void init();
        void input();
        void logic(); //FastNoise
        void render(RenderUtils &);

        std::map<SDL_Scancode, bool> keyStates;
        View view;
        int mouse[2];
        bool mousedown{false};
        WorldUtils WU;
        int lastTilesize;
    private:
        int starttime;
        //entt::registry registry;
        int deltaTime;
        int lastTime;
        int bound_entities;
        bool screenshot;

};
