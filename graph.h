#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <string>
#include "node.h"
#include "edge.h"

namespace graph {

template<typename T = int>
class Graph {

template<typename U>
friend std::ostream& operator<<(std::ostream& os, const Graph<U>& g);

private:
    std::string name;
    std::map<int, std::shared_ptr<Node<T>>> nodes;
    std::map<int, std::shared_ptr<Edge<T>>> edges;
    int nextNodeId;
    int nextEdgeId;
    bool isDirected;

    Graph* parentGraph;
    std::vector<Graph*> subGraphs;

    int generateNodeId();
    int generateEdgeId();

    void deepCopy(const Graph& other);

public:
    Graph(const std::string& name = "", bool directed = false);

    Graph(const Graph& other);

    Graph& operator=(const Graph& other);

    ~Graph();

    const std::string& getName() const;
    void setName(const std::string& newName);

    bool isEmpty() const;
    size_t getNodeCount() const;
    size_t getEdgeCount() const;
    bool isDirectedGraph() const;

    Node<T>* addNode(const T& data = T());

    Node<T>* addNodeWithId(int id, const T& data = T());

    bool removeNode(int nodeId);

    bool removeNode(Node<T>* node);

    Node<T>* getNode(int nodeId);

    const Node<T>* getNode(int nodeId) const;

    bool containsNode(int nodeId) const;

    Edge<T>* addEdge(int fromNodeId, int toNodeId, double weight = 1.0);

    Edge<T>* addEdge(Node<T>* fromNode, Node<T>* toNode, double weight = 1.0);

    bool removeEdge(int edgeId);

    bool removeEdge(Edge<T>* edge);

    Edge<T>* getEdge(int edgeId);

    const Edge<T>* getEdge(int edgeId) const;

    bool containsEdge(int edgeId) const;

    Edge<T>* findEdge(int fromNodeId, int toNodeId) const;

    Edge<T>* findEdge(const Node<T>* fromNode, const Node<T>* toNode) const;

    bool hasEdge(int fromNodeId, int toNodeId) const;

    bool hasEdge(const Node<T>* fromNode, const Node<T>* toNode) const;

    std::vector<Node<T>*> getNeighbors(int nodeId) const;

    std::vector<Node<T>*> getNeighbors(const Node<T>* node) const;

    std::vector<int> getNeighborIds(int nodeId) const;

    std::vector<Node<T>*> getAllNodes() const;

    std::vector<Edge<T>*> getAllEdges() const;

    std::vector<Edge<T>*> getEdgesFrom(int nodeId) const;

    std::vector<Edge<T>*> getEdgesTo(int nodeId) const;

    Graph* createSubGraph(const std::string& subGraphName = "");

    Graph* getParentGraph() const;

    std::vector<Graph*> getSubGraphs() const;

    Graph* getSubGraph(const std::string& subGraphName) const;

    Graph* getSubGraphAt(size_t index) const;

    size_t getSubGraphCount() const;

    bool removeSubGraph(Graph* subGraph);

    bool removeSubGraph(const std::string& subGraphName);

    bool removeSubGraphAt(size_t index);

    int getDepth() const;

    Graph* getRootGraph();

    const Graph* getRootGraph() const;

    bool isDescendantOf(const Graph* ancestor) const;

    bool isAncestorOf(const Graph* descendant) const;

    bool isRoot() const;

    bool isLeaf() const;

    void clear();

    void clearAll();

    void print(std::ostream& os = std::cout) const;

    void printHierarchy(std::ostream& os = std::cout, int indent = 0) const;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Graph<T>& g);

}

#endif
