
#include "path.h"
#include <stdio.h>
using namespace std;


vector<vector<int>> neighbors{
    {1,0}, {0,1}, {-1,0}, {0,-1},
    {1,-1}, {-1,-1}, {1,1}, {-1,1}
};

inline int heuristic(vector<int> a, vector<int> b) {
  return abs(a[0] - b[0]) + abs(a[1] - b[1]);
}

vector<int> reconstruct_path(int start, int goal, unordered_map<int, int> came_from) {
    if(start == goal) return vector<int>{-1};
    vector<int> path;
    int current = goal;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    //path.push_back(start); // optional
    reverse(path.begin(), path.end());
    return path;
}

void dijkstra(Graph graph, vector<vector<int>> nodes,
              int start, int goal,
              unordered_map<int, int>& came_from,
              unordered_map<int, int>& cost_so_far
              ) {
    PriorityQueue<int, int> frontier;
    frontier.put(start, 0);

    came_from[start] = 0;
    cost_so_far[start] = 0;

    while (!frontier.empty()) {
        int current = frontier.get();

        if (current == goal) {
            break;
        }

        for (int next : graph[current]) {
            int new_cost = cost_so_far[current] + 1; //graph.cost(current, next);
            if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                int priority = new_cost + heuristic(nodes[next], nodes[goal]);
                frontier.put(next, priority);
                came_from[next] = current;
            }
        }
    }
}

// vector<int> ConstructGraph(int tilesize, vector<int> origin, vector<int> src, int width, int height, vector<int> dest) {
void ConstructGraph(int width, int height, Graph & graph, vector<vector<int>> & nodes) {

    // int dest_index;
    // int obj_index;
    // Graph graph;
    //vector<vector<int>> nodes;
    nodes.clear();
    int index;
    for(int w = 0; w < width; ++w) {
        for(int h = 0; h < height; ++h) {
            nodes.push_back({w, h});
            ++index;
        }
    }

    vector<vector<int>>::iterator it;
    // it = find(nodes.begin(), nodes.end(), src);
    // if(it != nodes.end()) {
    //     obj_index = distance(nodes.begin(), it);
    // }
    // it = find(nodes.begin(), nodes.end(), dest);
    // if(it != nodes.end()) {
    //     dest_index = distance(nodes.begin(), it);
    // }
    // else return {-1,-1};
    //
    for(float i = 0; i < (width)*(height); ++i) {
        for(int n = 0; n < 8; ++n) {
            if(nodes[i][0] + neighbors[n][0] > -1 && nodes[i][0] + neighbors[n][0] < width) {
                if(nodes[i][1] + neighbors[n][1] > -1 && nodes[i][1] + neighbors[n][1] < height) {
                    vector<int> p{
                        nodes[i][0] + neighbors[n][0],
                        nodes[i][1] + neighbors[n][1]
                    };

                    it = find(nodes.begin(), nodes.end(), p);
                    if(it != nodes.end()) {
                        index = distance(nodes.begin(), it);
                        graph[i].push_back(index);
                    }
                }
            }

            // vector<int> p{
            //     nodes[i][0] + neighbors[n][0],
            //     nodes[i][1] + neighbors[n][1]
            // };

            // it = find(nodes.begin(), nodes.end(), p);
            // if(it != nodes.end()) {
            //     index = distance(nodes.begin(), it);
            //     graph.edges[i].push_back(index);
            // }
        }
    }


    //return graph;
}
