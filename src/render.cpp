#include "render.hpp"

RenderUtils::RenderUtils(
    SDL_Renderer *renderer,
    int w, int h, int t
    ) : renderer{renderer},
        WIDTH{w}, HEIGHT{h},tilesize{t},
        rect{0, 0, 0, 0},
        color{255, 0, 0, 255}{
            //noise.SetFrequency(.199931245);
            setColor(0, 55, 89, 125);
            setRect(0, 0, w, h);
            render();
        }

void RenderUtils::setRect(int x, int y, int w, int h) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
}

void RenderUtils::setColor(int r, int g, int b, int a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

void RenderUtils::render() const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderUtils::playerEntity(entt::registry & registry) {
    int tx, ty;
    setColor(0, 0, 0, 255);
    setRect(WIDTH/2, HEIGHT/2, tilesize/10, tilesize/10);
    render();
}

void RenderUtils::npcEntities(entt::registry & registry, int tilesize) {
    const auto view = registry.view<NPC, Position, Rendered, Identification>();
    setColor(255, 255, 255, 255);
    for(const auto npc : view) {
        Position pos = view.get<Position>(npc);
        Identification i = view.get<Identification>(npc);
        printf("%d\n", tilesize);
        setRect(pos.screenX, pos.screenY,
                tilesize * (static_cast<float>(i.size)/default_tilesize),
                tilesize * (static_cast<float>(i.size)/default_tilesize));
        render();

        if(registry.has<Health>(npc)) {
            const Health amt = registry.get<Health>(npc);
            if(amt.health != amt.maxHealth) {
                setRect(pos.screenX - 5, pos.screenY - 10, ((amt.health/amt.maxHealth) * 20) + 1, 5);
                setColor(255, 0, 0, 125);
                render();
                setColor(255, 255, 255, 255);
            }
        }
    }
}

void RenderUtils::rockEntities(entt::registry & registry, int tilesize) {
    const auto view = registry.view<Rock, Position, Rendered, Identification>();
    for(const auto rock : view) {
        Position pos = view.get<Position>(rock);
        Identification id = view.get<Identification>(rock);
        setColor(id.r, id.g, id.b, 255);
        setRect(pos.screenX, pos.screenY,
                tilesize * (static_cast<float>(id.size)/default_tilesize),
                tilesize * (static_cast<float>(id.size)/default_tilesize));
        render();

        if(registry.has<Health>(rock)) {
            const Health amt = registry.get<Health>(rock);
            if(amt.health != amt.maxHealth) {
                setRect(pos.screenX - 5, pos.screenY - 10, ((amt.health/amt.maxHealth) * 20) + 1, 5);
                setColor(255, 0, 0, 125);
                render();
                setColor(255, 255, 255, 255);
            }
        }
    }
}

void RenderUtils::viewBounds(View & view, WorldUtils& WU) {
    int tx, ty, ID, rgb;
    int r, g, b;
    float n;
    for(int x = view.bounds[0][0]; x < view.bounds[1][0] + 2; ++x) {
        for(int y = view.bounds[0][1]; y < view.bounds[3][1] + 2; ++y) {
            ID = hasher(std::to_string(x) + " " + std::to_string(y));
            tx = -view.xmod + (x - view.bounds[0][0])*view.tilesize;
            ty = -view.ymod + (y - view.bounds[0][1])*view.tilesize;

            n = WU.terrainGeneration(x, y);
            srand(ID);
            rgb = 256 * n;

            if(n < 0.45){
                r = 0; g = 123; b = 238; //water
            }
            else if(n < 0.49){ //sand
                r = rgb*1.8; g = rgb*1.7; b = rgb*1.2;
            }
            else if(n < 0.62){ //grass
                r = 139*n*1.2; g = 214*n*1.8; b = 74*1.5;
            }
            else { //stone

                if(n <= 0.75) {
                    r = 139*(n*1.6); g = 139*(n*1.6); b = 139*(n*1.6);
                }
                //Ore generation?
                else {
                    //float tempf = WU.terrain_noise.GetFrequency();

                    n = WU.oreGeneration(x, y);
                    rgb = 256 * n;
                    r = x+n*255*.9;
                    g = y+n*255*1.2;
                    b = x+y+n*255*1.7;
                }
            }


            setColor(r, g, b, 255);
            setRect(tx, ty, view.tilesize, view.tilesize);
            render();
        }
    }
}


void RenderUtils::text(const char* text) {

    TTF_Font *font = TTF_OpenFont("resources/Chillax-Regular.otf", 16);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
    SDL_Surface * surface = NULL;
    SDL_Texture * texture = NULL;

    setRect(0, 0, 50, 50);

    //string data{};
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    TTF_CloseFont(font);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

}

void RenderUtils::screenshot() {
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    SDL_SaveBMP(sshot, "../GameEngine/screenshot.bmp");
    SDL_FreeSurface(sshot);

    setColor(255, 255, 255, 255);
    setRect(0, 0, WIDTH, HEIGHT);
    render();
}

