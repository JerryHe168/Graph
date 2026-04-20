#ifndef EDGE_H
#define EDGE_H

namespace graph {

template<typename T>
class Graph;

template<typename T>
class Edge {
private:
    int id;
    int fromNodeId;
    int toNodeId;
    double weight;
    bool directed;
    Graph<T>* ownerGraph;

public:
    Edge(Graph<T>* owner, int id, int from, int to, double weight = 1.0, bool directed = false);
    
    int getId() const;
    int getFromNodeId() const;
    int getToNodeId() const;
    double getWeight() const;
    void setWeight(double newWeight);
    bool isDirected() const;
    Graph<T>* getOwnerGraph() const;
    
    int getOtherNodeId(int nodeId) const;
};

}

#endif
