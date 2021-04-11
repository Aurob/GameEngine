#pragma once
#include "render.hpp"
#include "view.hpp"
#include "movement.hpp"
#include "collisions.hpp"
#include "factories.hpp"
#include <map>
#include <SDL_timer.h>
#include <SDL_scancode.h>

class Game {
    public:
        Game(int, int, int);
        void init();
        void input(SDL_Scancode);
        void logic();
        void render(RenderUtils &);

        std::map<SDL_Scancode, bool> keyStates;
        View view;
        int mouse[2];
        bool mousedown{false};

    private:

        entt::registry registry;

        int deltaTime;
        int lastTime;

        int bound_entities;

};
