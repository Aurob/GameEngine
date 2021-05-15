#include "game.hpp"
#include <stdio.h>



Game::Game(const int WIDTH, const int HEIGHT, const int TILESIZE) : view(WIDTH, HEIGHT, TILESIZE){
    width = WIDTH;
    height = HEIGHT;
    //TODO
    //Move player creation to factories
    const auto player = registry.create();
    registry.emplace<Player>(player);
    registry.emplace<Position>(player, Position{0, 0, .size=TILESIZE/10});
    registry.emplace<Movement>(player, Movement{64*10});
    registry.emplace<Health>(player, Health{100, 100});
    //for(int i = 0; i < 500; ++i) {
    //    makeNPC(registry, rand() % 1000, rand() % 1000, TILESIZE);
    //}
    starttime = SDL_GetTicks();
    lastTime = starttime;
}

void Game::init() {
    printf("Game started!\n");
}

void Game::logic() { //FastNoise noise
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

    //Adds or removes the Rendered component to entities that are
    // newly visible or no longer visible
    view.updateEntities(registry, bound_entities);

    //Updates each entity's position
    //entityMovement(registry, view.tilesize, WU);
    if(bound_entities < 15) {
        for(auto spawn : WU.entitySpawn(5, view.bounds)) {
            makeRock(registry,
                     spawn[0]*view.default_tilesize,
                     spawn[1]*view.default_tilesize,
                     spawn[0]+spawn[2]*255*.9,
                     spawn[1]+spawn[2]*255*1.2,
                     spawn[0]+spawn[1]+spawn[2]*255*1.7,
                     rand()%view.default_tilesize);

            makeNPC(registry,
                spawn[0]*view.default_tilesize,
                spawn[1]*view.default_tilesize,
                view.default_tilesize);
        }
    }


    //Checks for user interaction on visible entities
    entityInteractions(mouse, mousedown, registry);

    //registry.sort<Rendered>([&registry](const entt::entity lhs, const entt::entity rhs) {
    //    const auto p1 = registry.get<Position>(lhs);
    //   const auto p2 = registry.get<Position>(rhs);
    //   return p1.screenY < p2.screenY && p1.screenX + p1.size < p2.screenX + p2.size;
    //});
}

void Game::render(RenderUtils &renderer) {
    //Render the visible tiles
    renderer.viewBounds(view, WU);

    renderer.rockEntities(registry, view.tilesize);

    //Render each visible entity
    renderer.npcEntities(registry, view.tilesize);



    //Draw the player sprite at the center of the screen
    renderer.playerEntity(registry);
    //

    //Render any UI elements
    renderer.text((std::to_string(deltaTime)+"ms").c_str(), 0, 0, 100);

    //HP Text
    const auto player = registry.view<Player>();
    for(const auto p : player) {
        Health &usrHP = registry.get<Health>(p);
        renderer.text((std::to_string(static_cast<int>(usrHP.health))+"/"+std::to_string(static_cast<int>(usrHP.maxHealth))+"HP").c_str(), 25, height-86, 150);

    }
    if(screenshot) {
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


