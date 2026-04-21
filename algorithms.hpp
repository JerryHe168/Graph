#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <vector>
#include <queue>
#include <stack>
#include <unordered_set>
#include "graph.hpp"

namespace graph {
namespace algo {

namespace {

// 模板辅助类，用于获取节点的邻居
template<typename NodePtr>
struct NodeTraitsHelper;

template<typename T>
struct NodeTraitsHelper<Node<T>*> {
    template<typename U>
    static Node<U>* getNode(const Graph<U>& graph, int nodeId) {
        return const_cast<Graph<U>&>(graph).getNode(nodeId);
    }

    template<typename U>
    static std::vector<Node<U>*> getNeighbors(const Graph<U>& graph, Node<U>* node) {
        return graph.getNeighbors(node);
    }
};

template<typename T>
struct NodeTraitsHelper<const Node<T>*> {
    template<typename U>
    static const Node<U>* getNode(const Graph<U>& graph, int nodeId) {
        return graph.getNode(nodeId);
    }

    template<typename U>
    static std::vector<const Node<U>*> getNeighbors(const Graph<U>& graph, const Node<U>* node) {
        return graph.getNeighborsConst(node);
    }
};

struct NodeTraits {
    template<typename NodePtr, typename T>
    static NodePtr getNode(const Graph<T>& graph, int nodeId) {
        return NodeTraitsHelper<NodePtr>::template getNode<T>(graph, nodeId);
    }

    template<typename NodePtr, typename T>
    static auto getNeighbors(const Graph<T>& graph, NodePtr node) -> decltype(NodeTraitsHelper<NodePtr>::template getNeighbors<T>(graph, node)) {
        return NodeTraitsHelper<NodePtr>::template getNeighbors<T>(graph, node);
    }
};

} // namespace

template<typename T, typename NodePtr>
std::vector<NodePtr> bfsImpl(const Graph<T>& graph, int startNodeId, std::unordered_set<int>* visited) {
    std::vector<NodePtr> result;
    NodePtr startNode = NodeTraits::getNode<NodePtr, T>(graph, startNodeId);
    if (!startNode) return result;

    std::unordered_set<int> localVisited;
    std::unordered_set<int>& visitSet = visited ? *visited : localVisited;
    std::queue<NodePtr> q;

    if (visitSet.find(startNodeId) == visitSet.end()) {
        q.push(startNode);
        visitSet.insert(startNodeId);
    }

    while (!q.empty()) {
        NodePtr current = q.front();
        q.pop();
        result.push_back(current);

        auto neighbors = NodeTraits::getNeighbors<NodePtr, T>(graph, current);
        for (auto neighbor : neighbors) {
            if (visitSet.find(neighbor->getId()) == visitSet.end()) {
                visitSet.insert(neighbor->getId());
                q.push(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
std::vector<Node<T>*> bfs(const Graph<T>& graph, int startNodeId) {
    return bfsImpl<T, Node<T>*>(graph, startNodeId, nullptr);
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
    return bfsImpl<T, Node<T>*>(graph, startNodeId, &visited);
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
std::vector<const Node<T>*> bfsConst(const Graph<T>& graph, int startNodeId) {
    return bfsImpl<T, const Node<T>*>(graph, startNodeId, nullptr);
}

template<typename T>
std::vector<const Node<T>*> bfsConst(const Graph<T>& graph, const Node<T>* startNode) {
    if (!startNode) {
        return std::vector<const Node<T>*>();
    }
    return bfsConst(graph, startNode->getId());
}

template<typename T>
std::vector<const Node<T>*> bfsConst(const Graph<T>& graph, int startNodeId, std::unordered_set<int>& visited) {
    return bfsImpl<T, const Node<T>*>(graph, startNodeId, &visited);
}

template<typename T>
std::vector<const Node<T>*> bfsConst(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<const Node<T>*>();
    }
    return bfsConst(graph, startNode->getId(), visited);
}

template<typename T, typename NodePtr>
std::vector<NodePtr> dfsImpl(const Graph<T>& graph, int startNodeId, std::unordered_set<int>* visited) {
    std::vector<NodePtr> result;
    NodePtr startNode = NodeTraits::getNode<NodePtr, T>(graph, startNodeId);
    if (!startNode) return result;

    std::unordered_set<int> localVisited;
    std::unordered_set<int>& visitSet = visited ? *visited : localVisited;
    std::stack<NodePtr> s;

    if (visitSet.find(startNodeId) == visitSet.end()) {
        s.push(startNode);
        visitSet.insert(startNodeId);
    }

    while (!s.empty()) {
        NodePtr current = s.top();
        s.pop();
        result.push_back(current);

        auto neighbors = NodeTraits::getNeighbors<NodePtr, T>(graph, current);
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
            auto neighbor = *it;
            if (visitSet.find(neighbor->getId()) == visitSet.end()) {
                visitSet.insert(neighbor->getId());
                s.push(neighbor);
            }
        }
    }

    return result;
}

template<typename T>
std::vector<Node<T>*> dfs(const Graph<T>& graph, int startNodeId) {
    return dfsImpl<T, Node<T>*>(graph, startNodeId, nullptr);
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
    return dfsImpl<T, Node<T>*>(graph, startNodeId, &visited);
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

template<typename T>
std::vector<const Node<T>*> dfsConst(const Graph<T>& graph, int startNodeId) {
    return dfsImpl<T, const Node<T>*>(graph, startNodeId, nullptr);
}

template<typename T>
std::vector<const Node<T>*> dfsConst(const Graph<T>& graph, const Node<T>* startNode) {
    if (!startNode) {
        return std::vector<const Node<T>*>();
    }
    return dfsConst(graph, startNode->getId());
}

template<typename T>
std::vector<const Node<T>*> dfsConst(const Graph<T>& graph, int startNodeId, std::unordered_set<int>& visited) {
    return dfsImpl<T, const Node<T>*>(graph, startNodeId, &visited);
}

template<typename T>
std::vector<const Node<T>*> dfsConst(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<const Node<T>*>();
    }
    return dfsConst(graph, startNode->getId(), visited);
}

namespace {

template<typename T, typename NodePtr, typename Result>
void dfsRecursiveHelperImpl(const Graph<T>& graph, NodePtr current,
                           std::unordered_set<int>& visited,
                           Result& result) {
    visited.insert(current->getId());
    result.push_back(current);

    auto neighbors = NodeTraits::getNeighbors<NodePtr, T>(graph, current);
    for (auto neighbor : neighbors) {
        if (visited.find(neighbor->getId()) == visited.end()) {
            dfsRecursiveHelperImpl(graph, neighbor, visited, result);
        }
    }
}

template<typename T>
void dfsRecursiveHelper(const Graph<T>& graph, Node<T>* current,
                         std::unordered_set<int>& visited,
                         std::vector<Node<T>*>& result) {
    dfsRecursiveHelperImpl(graph, current, visited, result);
}

template<typename T>
void dfsRecursiveHelperConst(const Graph<T>& graph, const Node<T>* current,
                              std::unordered_set<int>& visited,
                              std::vector<const Node<T>*>& result) {
    dfsRecursiveHelperImpl(graph, current, visited, result);
}

}

template<typename T, typename NodePtr, typename Result>
Result dfsRecursiveImpl(const Graph<T>& graph, int startNodeId, std::unordered_set<int>* visited) {
    Result result;
    NodePtr startNode = NodeTraits::getNode<NodePtr, T>(graph, startNodeId);
    if (!startNode) return result;

    std::unordered_set<int> localVisited;
    std::unordered_set<int>& visitSet = visited ? *visited : localVisited;

    if (visitSet.find(startNodeId) == visitSet.end()) {
        dfsRecursiveHelperImpl(graph, startNode, visitSet, result);
    }
    return result;
}

template<typename T>
std::vector<Node<T>*> dfsRecursive(const Graph<T>& graph, int startNodeId) {
    return dfsRecursiveImpl<T, Node<T>*, std::vector<Node<T>*>>(graph, startNodeId, nullptr);
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
    return dfsRecursiveImpl<T, Node<T>*, std::vector<Node<T>*>>(graph, startNodeId, &visited);
}

template<typename T>
std::vector<Node<T>*> dfsRecursive(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<Node<T>*>();
    }
    return dfsRecursive(graph, startNode->getId(), visited);
}

template<typename T>
std::vector<const Node<T>*> dfsRecursiveConst(const Graph<T>& graph, int startNodeId) {
    return dfsRecursiveImpl<T, const Node<T>*, std::vector<const Node<T>*>>(graph, startNodeId, nullptr);
}

template<typename T>
std::vector<const Node<T>*> dfsRecursiveConst(const Graph<T>& graph, const Node<T>* startNode) {
    if (!startNode) {
        return std::vector<const Node<T>*>();
    }
    return dfsRecursiveConst(graph, startNode->getId());
}

template<typename T>
std::vector<const Node<T>*> dfsRecursiveConst(const Graph<T>& graph, int startNodeId, std::unordered_set<int>& visited) {
    return dfsRecursiveImpl<T, const Node<T>*, std::vector<const Node<T>*>>(graph, startNodeId, &visited);
}

template<typename T>
std::vector<const Node<T>*> dfsRecursiveConst(const Graph<T>& graph, const Node<T>* startNode, std::unordered_set<int>& visited) {
    if (!startNode) {
        return std::vector<const Node<T>*>();
    }
    return dfsRecursiveConst(graph, startNode->getId(), visited);
}

}
}

#endif
