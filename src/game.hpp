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
#include <SDL2/SDL_image.h>

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
        int width, height;
        bool mousedown{false};
        WorldUtils WU;
        int lastTilesize;
    private:
        int starttime;
        entt::registry registry;
        int deltaTime;
        int lastTime;
        int bound_entities;
        int total_entities;
        bool screenshot;

};
