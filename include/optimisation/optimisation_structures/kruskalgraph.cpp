#include "kruskalgraph.hpp"

using namespace std;
using namespace homecare;

KruskalGraph::KruskalGraph(int N) {
    parent = new int[N];
    this -> N = N;
    //i 0 1 2 3 4 5
    //parent[i] 0 1 2 3 4 5
    for (int i = 0; i < N; i++)
        parent[i] = i;

    G.clear();
    T.clear();
}

void KruskalGraph::addWeightedEdge(int u, int v, int w) {
    G.push_back(make_pair(w, edge(u, v)));
}

int KruskalGraph::findSet(int i) {
    int representative = i;
    // If i is not the parent of itself then i is not the representative of his set, search for parent
    while (representative != parent[representative]) {
        representative = parent[representative];
    }
    // If i is the parent of itself
    return representative;
}

void KruskalGraph::mergeSet(int u, int v) {
    parent[u] = parent[v];
}

void KruskalGraph::generateKruskalTree() {
    int i, uRep, vRep;
    sort(G.begin(), G.end());  // increasing weight
    for (i = 0; i < G.size(); i++) {
        uRep = findSet(G[i].second.first);
        vRep = findSet(G[i].second.second);
        if (uRep != vRep) {
        T.push_back(G[i]);  // add to tree
        mergeSet(uRep, vRep);
        }
    }
}

void KruskalGraph::print() {
    cout << "Edge :" << " Weight" << '\n';
    for (int i = 0; i < T.size(); i++) {
        cout << T[i].second.first << " - " << T[i].second.second << " : "
        << T[i].first;
        cout << '\n';
    }
}

vector<int> KruskalGraph::getKruskalRank(int elementToDestroy) {
    generateKruskalTree();
    vector<int> sorted;
    sorted.push_back(T[0].second.first);

    for (int i = 0; i < sorted.size(); ++i) {
        for (int j = 0; j < T.size(); ++j) {
            if (T[j].second.first == sorted[i]) {
                addIfNotPresent(sorted, T[j].second.second);
            } else if (T[j].second.second == sorted[i]) {
                addIfNotPresent(sorted, T[j].second.first);
            }
        }
    }
    if (sorted.size() <= elementToDestroy) { return sorted; }
    vector<int> rankedElements(sorted.begin(), sorted.begin() + elementToDestroy);
    return rankedElements;
}

void KruskalGraph::addIfNotPresent(vector<int>& sorted, int value) {
    if (find(sorted.begin(), sorted.end(), value) == sorted.end()) {
        sorted.push_back(value);
    }
}
