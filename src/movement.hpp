#pragma once
#include <SDL_scancode.h>
#include <SDL_timer.h>
#include "entt.hpp"
#include "components.hpp"
#include <map>
#include <math.h>
#include <time.h>
#include "worldgen.hpp"
#include "FastNoise.h"

bool playerMovement(entt::registry &, std::map<SDL_Scancode, bool> &, int);
void entityMovement(entt::registry &, int);
