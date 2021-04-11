#pragma once
#include <SDL_render.h>
#include <SDL_ttf.h>
#include "entt.hpp"
#include "view.hpp"
#include "components.hpp"
#include <string> //for std::hash
#include "FastNoise.h"

class RenderUtils {
    public:
        RenderUtils(SDL_Renderer *, int, int, int);
        void setRect(int, int, int, int);
        void setColor(int, int, int, int);
        void playerEntity(entt::registry &);
        void npcEntities(entt::registry &);
        void viewBounds(View &);
        void screenshot();
        void text(const char*);
        void render() const;

    private:
        SDL_Renderer *renderer;
        int WIDTH, HEIGHT, tilesize;
        SDL_Rect rect;
        SDL_Color color;

        TTF_Font *font;
        std::hash<std::string> hasher;
        FastNoise noise;
};
