#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include "graph.hpp"

namespace graph {
namespace algo {

template<typename T>
std::vector<Node<T>*> bfs(const Graph<T>& graph, int startNodeId) {
    std::vector<Node<T>*> result;
    Node<T>* startNode = const_cast<Graph<T>&>(graph).getNode(startNodeId);
    if (!startNode) return result;

    std::unordered_set<int> visited;
    std::queue<Node<T>*> q;

    q.push(startNode);
    visited.insert(startNodeId);

    while (!q.empty()) {
        Node<T>* current = q.front();
        q.pop();
        result.push_back(current);

        std::vector<Node<T>*> neighbors = graph.getNeighbors(current);
        for (Node<T>* neighbor : neighbors) {
            if (visited.find(neighbor->getId()) == visited.end()) {
                visited.insert(neighbor->getId());
                q.push(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
std::vector<Node<T>*> bfs(const Graph<T>& graph, const Node<T>* startNode) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return bfs(graph, startNode->getId());
}

template<typename T>
std::vector<Node<T>*> bfs(const Graph<T>& graph, int startNodeId, std::unordered_set<int>& visited) {
    std::vector<Node<T>*> result;
    Node<T>* startNode = const_cast<Graph<T>&>(graph).getNode(startNodeId);
    if (!startNode) return result;

    std::queue<Node<T>*> q;

    if (visited.find(startNodeId) == visited.end()) {
        q.push(startNode);
        visited.insert(startNodeId);
    }

    while (!q.empty()) {
        Node<T>* current = q.front();
        q.pop();
        result.push_back(current);

        std::vector<Node<T>*> neighbors = graph.getNeighbors(current);
        for (Node<T>* neighbor : neighbors) {
            if (visited.find(neighbor->getId()) == visited.end()) {
                visited.insert(neighbor->getId());
                q.push(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
std::vector<Node<T>*> bfs(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return bfs(graph, startNode->getId(), visited);
}

template<typename T>
std::vector<int> bfsIds(const Graph<T>& graph, int startNodeId) {
    std::vector<Node<T>*> resultNodes = bfs(graph, startNodeId);
    std::vector<int> ids;
    for (Node<T>* node : resultNodes) {
        ids.push_back(node->getId());
    }
    return ids;
}

template<typename T>
std::vector<Node<T>*> dfs(const Graph<T>& graph, int startNodeId) {
    std::vector<Node<T>*> result;
    Node<T>* startNode = const_cast<Graph<T>&>(graph).getNode(startNodeId);
    if (!startNode) return result;

    std::unordered_set<int> visited;
    std::stack<Node<T>*> s;

    s.push(startNode);
    visited.insert(startNodeId);

    while (!s.empty()) {
        Node<T>* current = s.top();
        s.pop();
        result.push_back(current);

        std::vector<Node<T>*> neighbors = graph.getNeighbors(current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            Node<T>* neighbor = *it;
            if (visited.find(neighbor->getId()) == visited.end()) {
                visited.insert(neighbor->getId());
                s.push(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
std::vector<Node<T>*> dfs(const Graph<T>& graph, const Node<T>* startNode) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return dfs(graph, startNode->getId());
}

template<typename T>
std::vector<Node<T>*> dfs(const Graph<T>& graph, int startNodeId, std::unordered_set<int>& visited) {
    std::vector<Node<T>*> result;
    Node<T>* startNode = const_cast<Graph<T>&>(graph).getNode(startNodeId);
    if (!startNode) return result;

    std::stack<Node<T>*> s;

    if (visited.find(startNodeId) == visited.end()) {
        s.push(startNode);
        visited.insert(startNodeId);
    }

    while (!s.empty()) {
        Node<T>* current = s.top();
        s.pop();
        result.push_back(current);

        std::vector<Node<T>*> neighbors = graph.getNeighbors(current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            Node<T>* neighbor = *it;
            if (visited.find(neighbor->getId()) == visited.end()) {
                visited.insert(neighbor->getId());
                s.push(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
std::vector<Node<T>*> dfs(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return dfs(graph, startNode->getId(), visited);
}

template<typename T>
std::vector<int> dfsIds(const Graph<T>& graph, int startNodeId) {
    std::vector<Node<T>*> resultNodes = dfs(graph, startNodeId);
    std::vector<int> ids;
    for (Node<T>* node : resultNodes) {
        ids.push_back(node->getId());
    }
    return ids;
}

namespace {

template<typename T>
void dfsRecursiveHelper(const Graph<T>& graph, Node<T>* current,
                         std::unordered_set<int>& visited,
                         std::vector<Node<T>*>& result) {
    visited.insert(current->getId());
    result.push_back(current);

    std::vector<Node<T>*> neighbors = graph.getNeighbors(current);
    for (Node<T>* neighbor : neighbors) {
        if (visited.find(neighbor->getId()) == visited.end()) {
            dfsRecursiveHelper(graph, neighbor, visited, result);
        }
    }
}

}

template<typename T>
std::vector<Node<T>*> dfsRecursive(const Graph<T>& graph, int startNodeId) {
    std::vector<Node<T>*> result;
    Node<T>* startNode = const_cast<Graph<T>&>(graph).getNode(startNodeId);
    if (!startNode) return result;

    std::unordered_set<int> visited;
    dfsRecursiveHelper(graph, startNode, visited, result);
    return result;
}

template<typename T>
std::vector<Node<T>*> dfsRecursive(const Graph<T>& graph, const Node<T>* startNode) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return dfsRecursive(graph, startNode->getId());
}

template<typename T>
std::vector<Node<T>*> dfsRecursive(const Graph<T>& graph, int startNodeId, std::unordered_set<int>& visited) {
    std::vector<Node<T>*> result;
    Node<T>* startNode = const_cast<Graph<T>&>(graph).getNode(startNodeId);
    if (!startNode) return result;

    if (visited.find(startNodeId) == visited.end()) {
        dfsRecursiveHelper(graph, startNode, visited, result);
    }
    return result;
}

template<typename T>
std::vector<Node<T>*> dfsRecursive(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return dfsRecursive(graph, startNode->getId(), visited);
}

}
}

#endif
