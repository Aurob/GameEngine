#pragma once
#include <SDL_render.h>
#include <SDL_ttf.h>
//#include "entt.hpp"
#include "view.hpp"
#include "components.hpp"
#include <string> //for std::hash
#include "FastNoise.h"
#include "worldgen.hpp"
#include "textures.hpp"
#include <SDL2/SDL_image.h>

class RenderUtils {
    public:
        RenderUtils(SDL_Renderer *, int, int, int);
        void setRect(int, int, int, int);
        void setColor(int, int, int, int);
        void setImage(Texture, int, int, int, int, int, int, int, int);
        void playerEntity(entt::registry &);
        void npcEntities(entt::registry &, int);
        void rockEntities(entt::registry &, int);
        void viewBounds(View &, WorldUtils &);
        void screenshot();
        void text(const char*, int, int, int);
        void render() const;
        //FastNoise noise;

        TextureUtils TU;

    private:
        SDL_Renderer *renderer;
        int WIDTH, HEIGHT, tilesize;
        SDL_Rect rect1, rect2;
        SDL_Color color;

        TTF_Font *font;
        std::hash<std::string> hasher;
};
