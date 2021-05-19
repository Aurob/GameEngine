#include "render.hpp"

RenderUtils::RenderUtils(
    SDL_Renderer *renderer,
    int w, int h, int t
    ) : renderer{renderer},
        WIDTH{w}, HEIGHT{h},tilesize{t},
        rect1{0, 0, 0, 0},
        color{255, 0, 0, 255}{
            //noise.SetFrequency(.199931245);
            TU.renderer = renderer;
            TU.loadTextures();
            setColor(0, 55, 89, 125);
            setRect(0, 0, w, h);
            render();
        }

void RenderUtils::setRect(int x, int y, int w, int h) {
    rect1.x = x;
    rect1.y = y;
    rect1.w = w;
    rect1.h = h;
}

void RenderUtils::setColor(int r, int g, int b, int a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

void RenderUtils::setImage(Texture TU, int x, int y, int w, int h, int tx, int ty, int tw, int th) {
    rect1.x = tx; rect1.y = ty;
    rect1.w = tw; rect1.h = th;

    rect2.x = x; rect2.y = y;
    rect2.w = w; rect2.h = h;

    SDL_RenderCopy(renderer, TU.tex, &rect1, &rect2);
    rect1.x=0;rect1.y=0;rect2.x=0;rect2.y=0;
}

void RenderUtils::render() const {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect1);
}

void RenderUtils::playerEntity(entt::registry & registry) {
    const auto player = registry.view<Player, Position, Health>();
    for(const auto p : player) {
        Position &usrpos = player.get<Position>(p);
        //Player object
        setColor(0, 0, 0, 255);
        setRect(WIDTH/2, HEIGHT/2, usrpos.size, usrpos.size);
        render();

        //Player health
        const Health amt = registry.get<Health>(p);
        setColor(255, 0, 0, 125);
        setRect(25, HEIGHT - 25, (WIDTH-50) * (amt.health/amt.maxHealth), 25);
        render();
    }
}

void RenderUtils::npcEntities(entt::registry & registry, int tilesize) {
    const auto view = registry.view<NPC, Position, Rendered, Identification>();
    int tw, th;
    //setColor(255, 255, 255, 255);
    for(const auto npc : view) {
        Position pos = view.get<Position>(npc);
        if(registry.has<Health>(npc)) {
            const Health amt = registry.get<Health>(npc);
            if(amt.health != amt.maxHealth) {
                setColor(255, 0, 0, 255);
                setRect(pos.screenX - 5, pos.screenY - 10, ((amt.health/amt.maxHealth) * 20) + 1, 5);
                render();
            }
        }
        Textured texture = registry.get<Textured>(npc);
        //setRect(pos.screenX, pos.screenY,
        //        tilesize * (static_cast<float>(pos.size)/64),
        //        tilesize * (static_cast<float>(pos.size)/64));
        Movement behavior = registry.get<Movement>(npc);
        Identification id = registry.get<Identification>(npc);
        Health h = registry.get<Health>(npc);
        srand(id.ID);
        switch(behavior.type) {
        case cautious:
            if(!h.living) tw = 4;
            else {
                if(behavior.dx<0  && abs(behavior.dx)>abs(behavior.dy)) tw = 1;
                if(behavior.dx>0 && abs(behavior.dx)>abs(behavior.dy)) tw = 3;
                if(behavior.dy<0 && abs(behavior.dy)>abs(behavior.dx)) tw = 2;
                if(behavior.dy>0 && abs(behavior.dy)>abs(behavior.dx)) tw = 0;
            }

            setImage(TU.Textures[texture.type][texture.index], pos.screenX, pos.screenY, pos.sizeS, pos.sizeS,
                     (80*(rand()%11)) + 16*tw, (64*(rand()%12)+16) + 16*(behavior.step), 16, 16);
        case aggresive:
            if(!h.living) tw = 4;
            else {
                if(behavior.dx<0  && abs(behavior.dx)>abs(behavior.dy)) tw = 1;
                if(behavior.dx>0 && abs(behavior.dx)>abs(behavior.dy)) tw = 3;
                if(behavior.dy<0 && abs(behavior.dy)>abs(behavior.dx)) tw = 2;
                if(behavior.dy>0 && abs(behavior.dy)>abs(behavior.dx)) tw = 0;
            }
            setImage(TU.Textures[texture.type][texture.index], pos.screenX, pos.screenY, pos.sizeS, pos.sizeS,
                     (80*(rand()%11)) + 16*tw, (64*(rand()%12)+16) + 16*(behavior.step), 16, 16);
        default:
            render();
            break;
        }
    }
}

void RenderUtils::rockEntities(entt::registry & registry, int tilesize) {
    const auto view = registry.view<Rock, Position, Rendered, Identification>();
    for(const auto rock : view) {
        Position pos = view.get<Position>(rock);
        Identification id = view.get<Identification>(rock);
        Textured texture = registry.get<Textured>(rock);
        //Image
        if(registry.has<Health>(rock)) {
            const Health amt = registry.get<Health>(rock);
            if(amt.health != amt.maxHealth) {
                setRect(pos.screenX - 5, pos.screenY - 10, ((amt.health/amt.maxHealth) * 20) + 1, 5);
                setColor(255, 0, 0, 125);
                setColor(255, 255, 255, 255);
            }
        }
        setImage(TU.Textures[texture.type][texture.index], pos.screenX, pos.screenY, pos.sizeS, pos.sizeS, 0, 0, TU.Textures[texture.type][texture.index].w, TU.Textures[texture.type][texture.index].h);
        render();
        /*//Colored square
        setColor(id.r, id.g, id.b, 255);
        setRect(pos.screenX, pos.screenY,
                pos.sizeS, pos.sizeS);

        */

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

                if(n <= 0.70) {
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


void RenderUtils::text(const char* text, int x, int y, int width) {

    TTF_Font *font = TTF_OpenFont("resources/Chillax-Regular.otf", 16);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
    SDL_Surface * surface = NULL;
    SDL_Texture * texture = NULL;

    setRect(x, y, width, width/3);

    //string data{};
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect1);

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

