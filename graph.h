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

namespace graph {

template<typename T = int>
class Graph {
public:
    class Node {
    private:
        T data;
        int id;
        std::set<int> incomingEdges;
        std::set<int> outgoingEdges;
        Graph* ownerGraph;
    
    public:
        Node(Graph* owner, int id, const T& data);
        
        int getId() const;
        T& getData();
        const T& getData() const;
        void setData(const T& newData);
        Graph* getOwnerGraph() const;
        
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

    class Edge {
    private:
        int id;
        int fromNodeId;
        int toNodeId;
        double weight;
        bool directed;
        Graph* ownerGraph;
    
    public:
        Edge(Graph* owner, int id, int from, int to, double weight = 1.0, bool directed = false);
        
        int getId() const;
        int getFromNodeId() const;
        int getToNodeId() const;
        double getWeight() const;
        void setWeight(double newWeight);
        bool isDirected() const;
        Graph* getOwnerGraph() const;
        
        int getOtherNodeId(int nodeId) const;
    };

private:
    std::string name;
    std::map<int, std::shared_ptr<Node>> nodes;
    std::map<int, std::shared_ptr<Edge>> edges;
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
    
    Node* addNode(const T& data = T());
    
    Node* addNodeWithId(int id, const T& data = T());
    
    bool removeNode(int nodeId);
    
    bool removeNode(Node* node);
    
    Node* getNode(int nodeId);
    
    const Node* getNode(int nodeId) const;
    
    bool containsNode(int nodeId) const;
    
    Edge* addEdge(int fromNodeId, int toNodeId, double weight = 1.0);
    
    Edge* addEdge(Node* fromNode, Node* toNode, double weight = 1.0);
    
    bool removeEdge(int edgeId);
    
    bool removeEdge(Edge* edge);
    
    Edge* getEdge(int edgeId);
    
    const Edge* getEdge(int edgeId) const;
    
    bool containsEdge(int edgeId) const;
    
    Edge* findEdge(int fromNodeId, int toNodeId) const;
    
    Edge* findEdge(const Node* fromNode, const Node* toNode) const;
    
    bool hasEdge(int fromNodeId, int toNodeId) const;
    
    bool hasEdge(const Node* fromNode, const Node* toNode) const;
    
    std::vector<Node*> getNeighbors(int nodeId) const;
    
    std::vector<Node*> getNeighbors(const Node* node) const;
    
    std::vector<int> getNeighborIds(int nodeId) const;
    
    std::vector<Node*> getAllNodes() const;
    
    std::vector<Edge*> getAllEdges() const;
    
    std::vector<Edge*> getEdgesFrom(int nodeId) const;
    
    std::vector<Edge*> getEdgesTo(int nodeId) const;
    
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
    
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Graph<U>& g);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Graph<T>& g);

}

#endif
