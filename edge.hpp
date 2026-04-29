#ifndef EDGE_HPP
#define EDGE_HPP

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

template<typename T>
Edge<T>::Edge(Graph<T>* owner, int id, int from, int to, double weight, bool directed)
    : ownerGraph(owner), id(id), fromNodeId(from), toNodeId(to),
      weight(weight), directed(directed) {}

template<typename T>
int Edge<T>::getId() const { return id; }

template<typename T>
int Edge<T>::getFromNodeId() const { return fromNodeId; }

template<typename T>
int Edge<T>::getToNodeId() const { return toNodeId; }

template<typename T>
double Edge<T>::getWeight() const { return weight; }

template<typename T>
void Edge<T>::setWeight(double newWeight) { weight = newWeight; }

template<typename T>
bool Edge<T>::isDirected() const { return directed; }

template<typename T>
Graph<T>* Edge<T>::getOwnerGraph() const { return ownerGraph; }

template<typename T>
int Edge<T>::getOtherNodeId(int nodeId) const {
    if (nodeId == fromNodeId) return toNodeId;
    if (nodeId == toNodeId) return fromNodeId;
    return -1;
}

}

#endif