#include "game.hpp"
#include <stdio.h>

Game::Game(const int WIDTH, const int HEIGHT, const int TILESIZE) : view(WIDTH, HEIGHT, TILESIZE){
    //TODO
    //Move player creation to factories
    const auto player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Position>(player, Position{0, 0});
    registry.emplace<Movement>(player, Movement{256});

    for(int i = 0; i < 5000; ++i) {
        makeNPC(registry, rand() % 5000, rand() % 5000, TILESIZE);
    }

    lastTime = SDL_GetTicks();
}

void Game::init() {
    printf("Game started!\n");
}

void Game::input(const SDL_Scancode key) {
    keyStates[key] = true;
}

void Game::logic() {
    deltaTime = (SDL_GetTicks() - lastTime);
    lastTime = SDL_GetTicks();

    //Update the player's global position
    //Then use the player's updated position
    // to set the view bounds
    playerMovement(registry, keyStates, deltaTime);
    entityMovement(registry, view.tilesize);
    view.update(registry);
    view.updateEntities(registry);
    entityInteractions(mouse, mousedown, registry);
}

void Game::render(RenderUtils &renderer) {
    renderer.viewBounds(view);
    renderer.npcEntities(registry);
    renderer.playerEntity(registry);
}
