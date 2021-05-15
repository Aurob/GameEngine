
#include "textures.hpp"


//TODO
//set file paths and types in a separate file
//read from that file to load textures
std::vector<std::string> misc {
    "resources/01_dish.png",
    "resources/02_dish_2.png",
    "resources/04_bowl.png",
    "resources/05_apple_pie.png",
    "resources/06_apple_pie_dish.png",
    "resources/07_bread.png",
    "resources/08_bread_dish.png",
    "resources/09_baguette.png",
    "resources/100_taco_dish.png",
    "resources/101_waffle.png",
    "resources/102_waffle_dish.png",
    "resources/10_baguette_dish.png",
    "resources/11_bun.png",
    "resources/13_bacon.png",
    "resources/14_bacon_dish.png",
    "resources/15_burger.png",
    "resources/16_burger_dish.png",
    "resources/17_burger_napkin.png",
    "resources/18_burrito.png",
    "resources/19_burrito_dish.png",
    "resources/20_bagel.png",
    "resources/21_bagel_dish.png",
    "resources/22_cheesecake.png",
    "resources/23_cheesecake_dish.png",
    "resources/24_cheesepuff.png",
    "resources/26_chocolate.png",
    "resources/27_chocolate_dish.png",
    "resources/28_cookies.png",
    "resources/29_cookies_dish.png",
    "resources/30_chocolatecake.png",
    "resources/31_chocolatecake_dish.png",
    "resources/32_curry.png",
    "resources/33_curry_dish.png",
    "resources/34_donut.png",
    "resources/35_donut_dish.png",
    "resources/36_dumplings.png",
    "resources/37_dumplings_dish.png",
    "resources/38_friedegg.png",
    "resources/39_friedegg_dish.png",
    "resources/40_eggsalad.png",
    "resources/41_eggsalad_bowl.png",
    "resources/42_eggtart.png",
    "resources/43_eggtart_dish.png",
    "resources/44_frenchfries.png",
    "resources/45_frenchfries_dish.png",
    "resources/46_fruitcake.png",
    "resources/47_fruitcake_dish.png",
    "resources/48_garlicbread.png",
    "resources/49_garlicbread_dish.png",
    "resources/50_giantgummybear.png",
    "resources/51_giantgummybear_dish.png",
    "resources/52_gingerbreadman.png",
    "resources/53_gingerbreadman_dish.png",
    "resources/54_hotdog.png",
    "resources/55_hotdog_sauce.png",
    "resources/56_hotdog_dish.png",
    "resources/57_icecream.png",
    "resources/58_icecream_bowl.png",
    "resources/59_jelly.png",
    "resources/60_jelly_dish.png",
    "resources/61_jam.png",
    "resources/62_jam_dish.png",
    "resources/63_lemonpie.png",
    "resources/64_lemonpie_dish.png",
    "resources/65_loafbread.png",
    "resources/66_loafbread_dish.png",
    "resources/67_macncheese.png",
    "resources/68_macncheese_dish.png",
    "resources/69_meatball.png",
    "resources/70_meatball_dish.png",
    "resources/71_nacho.png",
    "resources/72_nacho_dish.png",
    "resources/73_omlet.png",
    "resources/74_omlet_dish.png",
    "resources/75_pudding.png",
    "resources/76_pudding_dish.png",
    "resources/77_potatochips.png",
    "resources/78_potatochips_bowl.png",
    "resources/79_pancakes.png",
    "resources/80_pancakes_dish.png",
    "resources/81_pizza.png",
    "resources/82_pizza_dish.png",
    "resources/83_popcorn.png",
    "resources/84_popcorn_bowl.png",
    "resources/85_roastedchicken.png",
    "resources/86_roastedchicken_dish.png",
    "resources/87_ramen.png",
    "resources/88_salmon.png",
    "resources/89_salmon_dish.png",
    "resources/90_strawberrycake.png",
    "resources/91_strawberrycake_dish.png",
    "resources/92_sandwich.png",
    "resources/93_sandwich_dish.png",
    "resources/94_spaghetti.png",
    "resources/95_steak.png",
    "resources/96_steak_dish.png",
    "resources/97_sushi.png",
    "resources/98_sushi_dish.png",
    "resources/99_taco.png"
};
std::vector<std::string> monsters {
    "resources/chartiles.png"
};
std::unordered_map<std::string, std::vector<std::string>> texture_types{
    {"misc", misc}, {"chars", monsters}
};

//,{"items", image_files} {"animals", animal_files},
bool TextureUtils::loadTextures(){
    printf("Loading Textures...\n");
    if(texturesLoaded) {
        printf(" Textures Loaded.\n");
        return true;
    }
    else{
        SDL_Texture *img = NULL;
        for(auto tex_type : texture_types){
            for(int i = 0; i < tex_type.second.size(); ++i){

                img = IMG_LoadTexture(renderer, tex_type.second[i].c_str());
                //printf((img == NULL) ? "Image failed to load\n" : "Good\n");
                Textures[tex_type.first].push_back(Texture{tex_type.second[i].c_str(), 0, 0, img});
                SDL_QueryTexture(Textures[tex_type.first][i].tex, NULL, NULL,
                    &Textures[tex_type.first][i].w, &Textures[tex_type.first][i].h
                );
            }
        }

        if(Textures.size() == texture_types.size()){
            texturesLoaded = true;
            printf(" Textures Loaded.\n");
            return true;
        }
        else return false;
    }
}

