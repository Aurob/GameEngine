#include "worldgen.hpp"

WorldUtils::WorldUtils(){
    //Set noise specific presets
    terrain_noise.SetSeed(time(NULL));
    step_noise.SetSeed(time(NULL));
    ore_noise.SetSeed(time(NULL));
    terrain_noise.SetFrequency(.013000123);
    step_noise.SetFrequency(.9123124512312);
    ore_noise.SetFrequency(.009);

}

float WorldUtils::terrainGeneration(int i, int j){
    //
    n = (terrain_noise.GetSimplexFractal((i), (j)) - -1) / (1 - -1);
    //n = (terrain_noise.GetSimplexFractal((i)+pow(n,2), (j)+pow(n,2)) - -1) / (1 - -1);

    return n;
}

float WorldUtils::stepGeneration(int i, int j){
    //
    n = (terrain_noise.GetSimplexFractal((i), (j)) - -1) / (1 - -1);
    return n;
}

float WorldUtils::oreGeneration(int i, int j){
    //
    n = (ore_noise.GetSimplexFractal((i), (j)) - -1) / (1 - -1);
    return n;
}

vector<vector<int>> WorldUtils::entitySpawn(int t, int bounds[4][2]) {
    vector<vector<int>> locations;
    for(int x = bounds[0][0]; x < bounds[1][0] + 2; ++x) {
        for(int y = bounds[0][1]; y < bounds[3][1] + 2; ++y) {
            if(terrainGeneration(x, y) > 0.75) {
                n = oreGeneration(x, y);
                if(n > .4) {
                    locations.push_back({x, y, n});
                }
            }
        }
    }


    return locations;
}
