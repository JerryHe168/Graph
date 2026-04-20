#ifndef NODE_H
#define NODE_H

#include <set>

namespace graph {

template<typename T>
class Graph;

template<typename T>
class Node {
private:
    T data;
    int id;
    std::set<int> incomingEdges;
    std::set<int> outgoingEdges;
    Graph<T>* ownerGraph;

public:
    Node(Graph<T>* owner, int id, const T& data);
    
    int getId() const;
    T& getData();
    const T& getData() const;
    void setData(const T& newData);
    Graph<T>* getOwnerGraph() const;
    
    const std::set<int>& getIncomingEdges() const;
    const std::set<int>& getOutgoingEdges() const;
    
    size_t getInDegree() const;
    size_t getOutDegree() const;
    size_t getDegree() const;
    
    void addIncomingEdge(int edgeId);
    void addOutgoingEdge(int edgeId);
    void removeIncomingEdge(int edgeId);
    void removeOutgoingEdge(int edgeId);
};

}

#endif
