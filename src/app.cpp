#include <SDL2/SDL.h>
#include "app.hpp"
#include "game.hpp"
#include <SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#define WIDTH 1280
#define HEIGHT 768
#define TILESIZE 64

Application::Application() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
}

Application::~Application() {
    SDL_Quit();
}

void Application::run() {
    SDL_Window * window = SDL_CreateWindow(
        "Game!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer * renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    RenderUtils game_renderer{renderer, WIDTH, HEIGHT, TILESIZE};
    SDL_RenderPresent(renderer);

    Game game(WIDTH, HEIGHT, TILESIZE);
    game.init();

    int frame = 0;
    bool quit = false;

    while(!quit) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            //TODO
            //Should this be moved to its own input.hpp/.cpp file?
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            }
            else if (e.type == SDL_KEYDOWN) {
                game.keyStates[e.key.keysym.scancode] = true;
            }
            else if (e.type == SDL_KEYUP) {
                game.keyStates[e.key.keysym.scancode] = false;
            }
            else if(e.type == SDL_MOUSEWHEEL) {
                if(e.wheel.y < 0 && game.view.tilesize > 2) {
                    game.view.tilesize/=2;
                }
                if(e.wheel.y > 0 && game.view.tilesize < 256) {
                    game.view.tilesize*=2;
                }
            }
            else if(e.type == SDL_MOUSEMOTION) {
                game.mouse[0] = e.motion.x;
                game.mouse[1] = e.motion.y;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN) {
                game.mousedown = true;
            }
            else if(e.type == SDL_MOUSEBUTTONUP) {
                game.mousedown = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        game.logic();
        game.render(game_renderer);
        ++frame;
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
