#include <unordered_map>
#include <vector>
#include <math.h>
#include <queue>
#include <algorithm>

using namespace std;

inline int heuristic(vector<int>, vector<int>);

typedef std::unordered_map<int, std::vector<int>> Graph;

template<typename T, typename priority_t>
struct PriorityQueue {
  typedef pair<priority_t, T> PQElement;
  priority_queue<PQElement, vector<PQElement>,
                 greater<PQElement>> elements;

  inline bool empty() const {
     return elements.empty();
  }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

vector<int> reconstruct_path(int, int, unordered_map<int, int>);
void dijkstra(Graph, vector<vector<int>>, int, int, unordered_map<int, int>&, unordered_map<int, int>&);
void ConstructGraph(int, int, Graph &, vector<vector<int>> &);
