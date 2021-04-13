#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <time.h>
#include "FastNoise.h"

using namespace std;

class WorldUtils {
    public:
        float n;
        FastNoise terrain_noise;
        WorldUtils();

        float terrainGeneration(int, int);
};
