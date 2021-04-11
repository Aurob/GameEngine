#include "input.hpp"
#include <stdio.h>

void playerInput(std::map<SDL_Scancode, bool> keys) {
}

bool mouseCollision(int mouse[2], Position entity) {
    if(mouse[0] > entity.screenX && mouse[0] < entity.screenX + 10) {
        if(mouse[1] > entity.screenY && mouse[1] < entity.screenY + 10) {
            return true;
        }
    }
    return false;
}
