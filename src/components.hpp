#pragma once
#include "path.h"

//Tag components
struct Player {};
struct NPC {};
struct Rock {};
struct Rendered {};
struct Destructing {};
struct Textured {
    string type;
    int index;
};

struct Interaction {
    bool damageOnCollision;
};

//global positions are the main position values
//screen and tile positions are calculated from the global
struct Position {
    int globalX, globalY;
    int tileGX, tileGY; // global
    int screenX, screenY;
    int tileSX, tileSY; // screen
    int size; //.....
    int sizeS; // altered size for zooming
    int direction;
};


struct Identification {
    int ID;
    int r, g, b;
};

struct Health {
    float maxHealth;
    float health;
    bool living;
    int timer;
};

enum Behaviour { cautious, aggresive, neutral };
struct Movement {
    float speed;
    float timer;
    int dx, dy;
    Behaviour type;
    int step;
};

struct Pathing {//
    vector<vector<int>> neighbors{
        {1,0}, {0,1}, {-1,0}, {0,-1},
        {1,-1}, {-1,-1}, {1,1}, {-1,1}
    };
    Graph path_graph{};
    vector<vector<int>> nodes{};
    std::unordered_map<int, int> came_from;
    std::unordered_map<int, int> cost_so_far;
    vector<vector<int>>::iterator it;
    int draw_distance;
    int idle_time;
    int src;

    Pathing(int size) {
        draw_distance = size;
        ConstructGraph(size, size, path_graph, nodes);
    }

    vector<int> path(vector<int> src, vector<int> dest) {
        came_from.clear();
        cost_so_far.clear();

        int destindex;
        int srcindex = ((draw_distance*draw_distance)/2) + ((draw_distance%2==0)?draw_distance/2:0);
        //printf("%d\n", dest[0]-src[0]);
        if(abs(dest[0]-src[0]) <= draw_distance) {
            if(abs(dest[1]-src[1]) <= draw_distance) {
                it = find(nodes.begin(), nodes.end(), vector<int>{(src[0]-dest[0]), (src[1]-dest[1])});
                if(it != nodes.end()) {
                    destindex = distance(nodes.begin(), it);
                }
            }
        }
        else return {-1, -1};

        dijkstra(path_graph, nodes, srcindex, destindex, came_from, cost_so_far);
        int next_index = reconstruct_path(srcindex, destindex, came_from)[0];
        if(next_index != -1)
            return {
                nodes[next_index][0],
                nodes[next_index][1]
            };
        else return {-1,-1};
    }
};



