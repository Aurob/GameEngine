#include "render.hpp"

RenderUtils::RenderUtils(
    SDL_Renderer *renderer,
    int w, int h, int t
    ) : renderer{renderer},
        WIDTH{w}, HEIGHT{h},tilesize{t},
        rect{0, 0, 0, 0},
        color{255, 0, 0, 255}{
            noise.SetFrequency(.199931245);
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
    setColor(0, 0, 0, 255);
    setRect(WIDTH/2, HEIGHT/2, tilesize/10, tilesize/10);
    render();

    const auto player = registry.view<Player, Position>();
    for(const auto p : player) {
        Position pos = registry.get<Position>(p);
    }
}

void RenderUtils::npcEntities(entt::registry & registry) {
    const auto view = registry.view<NPC, Position, Rendered>();
    setColor(255, 255, 255, 255);
    for(const auto npc : view) {
        Position pos = view.get<Position>(npc);
        setRect(pos.screenX, pos.screenY, tilesize/10, tilesize/10);
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

void RenderUtils::viewBounds(View & view) {
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
                    float tempf = noise.GetFrequency();
                    noise.SetFrequency(tempf/100);
                    rgb = 256 * (noise.GetCubicFractal((x), (y)) - -1) / (1 - -1);
                    r = rgb*((x+y)%256); g = rgb*((x+y)%256); b = rgb*((x+y)%256);
                    noise.SetFrequency(tempf);
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

