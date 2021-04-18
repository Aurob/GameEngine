#include <stdio.h>
#include <exception>
#include <SDL.h>
#include "app.hpp"

extern "C" int main(int, char *[]) {
  try {
      Application app;
      app.run();
  } catch (std::exception &e) {
    // The only exceptions we should get are from SDL
    printf("Error: %s\n", e.what());
    return 1;
  }
  return 0;
}
