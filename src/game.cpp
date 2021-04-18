#include "game.hpp"
#include <stdio.h>



Game::Game(const int WIDTH, const int HEIGHT, const int TILESIZE) : view(WIDTH, HEIGHT, TILESIZE){
    //TODO
    //Move player creation to factories
    const auto player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Position>(player, Position{0, 0});
    registry.emplace<Movement>(player, Movement{64*10});

    //for(int i = 0; i < 500; ++i) {
    //    makeNPC(registry, rand() % 1000, rand() % 1000, TILESIZE);
    //}
    starttime = SDL_GetTicks();
    lastTime = starttime;
}

void Game::init() {
    printf("Game started!\n");
}

void Game::logic(FastNoise noise) {
    //Set the time elapsed since the last frame
    deltaTime = (SDL_GetTicks() - lastTime);
    lastTime = SDL_GetTicks();

    //Checks for custom keybinds
    // P : screen shot
    // M : Map view
    // etc..
    input();

    //Update the player's global position
    //  unless moving onto an unaccessable tile
    if (playerMovement(registry, keyStates, deltaTime, WU)) {
        //use the player's updated position to set the view bounds
        view.update(registry);
    }

    //Updates each entity's position
    entityMovement(registry, view.tilesize, WU);

    if(bound_entities < 10) {
        for(auto spawn : WU.entitySpawn(5, view.bounds)) {
            makeRock(registry,
                     spawn[0]*view.default_tilesize,
                     spawn[1]*view.default_tilesize,
                     spawn[0]+spawn[2]*255*.9,
                     spawn[1]+spawn[2]*255*1.2,
                     spawn[0]+spawn[1]+spawn[2]*255*1.7);

            makeNPC(registry,
                    spawn[0]*view.default_tilesize,
                    spawn[1]*view.default_tilesize,
                    view.default_tilesize);
        }
    }

    //Adds or removes the Rendered component to entities that are
    // newly visible or no longer visible
    view.updateEntities(registry, bound_entities);
    //Checks for user interaction on visible entities
    entityInteractions(mouse, mousedown, registry);
}

void Game::render(RenderUtils &renderer) {

    //Render the visible tiles
    renderer.viewBounds(view, WU);

    //Render each visible entity
    renderer.npcEntities(registry);

    renderer.rockEntities(registry);

    //Draw the player sprite at the center of the screen
    renderer.playerEntity(registry);

    //Render any UI elements
    renderer.text((std::to_string(deltaTime)+"ms").c_str());

    if(screenshot) {
            printf("say chees!\n");
        renderer.screenshot();
        screenshot = false;
    }
}

void Game::input() {
    if(keyStates[SDL_SCANCODE_P]) {
        keyStates[SDL_SCANCODE_P] = false;
        screenshot = true;
    }

    if(keyStates[SDL_SCANCODE_M]) {
        keyStates[SDL_SCANCODE_M] = !keyStates[SDL_SCANCODE_M];
        //if(keyStates[SDL_SCANCODE_M]) {
            lastTilesize = view.tilesize;
            view.tilesize = 3.14159;
    }
}


