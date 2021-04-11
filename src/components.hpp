#pragma once
#include "path.h"

//Tag components
struct Player {};
struct NPC {};
struct Rendered {};
struct Interaction {};

//global positions are the main position values
//screen and tile positions are calculated from the global
struct Position {
    int globalX, globalY;
    int tileGX, tileGY; // global
    int screenX, screenY;
    int tileSX, tileSY; // screen
};

struct Identification {
    int ID;
};

struct Movement {
    float speed;
};

struct Health {
    float maxHealth;
    float health;
};

struct Pathing {//
    Graph path_graph{};
    vector<vector<int>> nodes{};
    std::unordered_map<int, int> came_from;
    std::unordered_map<int, int> cost_so_far;
    vector<vector<int>>::iterator it;
    int draw_distance;
    int idle_time;

    Pathing(int size) {
        draw_distance = size;
        setGraph(size);
    }

    void setGraph(int size){
        draw_distance = size;
        ConstructGraph(size, size, path_graph, nodes);
    }

    vector<int> path(vector<int> src, vector<int> dest) {
        came_from.clear();
        cost_so_far.clear();
        int srcindex;
        int destindex;
        if(abs(dest[0]-src[0]) < draw_distance/2) {
            if(abs(dest[1]-src[1]) < draw_distance/2) {
                it = find(nodes.begin(), nodes.end(), vector<int>{dest[0]-src[0], dest[1]-src[1]});
                if(it != nodes.end()) {
                    destindex = distance(nodes.begin(), it);
                }
            }
        }
        else return {-1, -1};

        dijkstra(path_graph, nodes, (draw_distance*draw_distance)/2, destindex, came_from, cost_so_far);
        int next_index = reconstruct_path((draw_distance*draw_distance)/2, destindex, came_from)[0];
        if(next_index != -1)
            return nodes[next_index];
        else return {-1,-1};
    }
};



