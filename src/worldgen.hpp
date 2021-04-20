#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <time.h>
#include "math.h"
#include "FastNoise.h"
#include "globals.hpp"
using namespace std;
extern int TILESIZE;
class WorldUtils {
    public:
        float n;
        FastNoise terrain_noise;
        FastNoise step_noise;
        FastNoise ore_noise;
        WorldUtils();

        float terrainGeneration(int, int);
        float stepGeneration(int, int);
        float oreGeneration(int, int);
        vector<vector<int>> entitySpawn(int, int[4][2]);
};
