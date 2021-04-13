#include "worldgen.hpp"

WorldUtils::WorldUtils(){
    //Set noise specific presets
    terrain_noise.SetFrequency(.008321123);
}

float WorldUtils::terrainGeneration(int i, int j){
    //
    // n = terrain_noise.GetPerlinFractal(i, j, 0, 1, 1, 0);
    //n = (terrain_noise.GetPerlinFractal((i), (j)) - -1) / (1 - -1);
    //n = (terrain_noise.GetPerlinFractal((i)+pow(n,2), (j)+pow(n,2)) - -1) / (1 - -1);

    return (terrain_noise.GetSimplexFractal((i), (j)) - -1) / (1 - -1);
}
