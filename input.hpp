#pragma once
#include <SDL_scancode.h>
#include "entt.hpp"
#include "components.hpp"
#include <map>

void playerInput(entt::registry &, std::map<SDL_Scancode, bool>);
bool mouseCollision(int[2], Position);
