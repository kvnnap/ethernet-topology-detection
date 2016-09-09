//
// Created by kevin on 7/25/16.
//

#ifndef NETWORK_DISCOVERY_INDEXEDTOPOLOGYTREE_H
#define NETWORK_DISCOVERY_INDEXEDTOPOLOGYTREE_H

#include <ostream>
#include <vector>
#include <map>
#include <set>

namespace Network {

    class IndexedTopologyNode {
    public:
        IndexedTopologyNode(size_t p_val = 0);

        bool isLeaf() const;
        bool isRoot() const;
        void setParent(size_t parentIndex);
        void deleteChild(size_t childIndex);

        std::vector<std::size_t> children;
        std::size_t parent;
        std::size_t val;
        bool parentSet;

        // When in connection with first, violations in second apply
        std::set<std::size_t> violators;
        //std::set<std::size_t> childVals;
    };

    std::ostream& operator<< (std::ostream& strm, const IndexedTopologyNode& indexedTopologyNode);

    class IndexedTopologyTree {
    public:
        size_t getNewNode();
        size_t addNewNode(const IndexedTopologyNode& node);
        IndexedTopologyNode& getNode(size_t index);
        void addChildToParent(size_t childIndex, size_t parentIndex);
        void moveNodeToNode(size_t nodeIndex, size_t parentIndex);
        void recomputeNodeState(size_t nodeIndex);
        void clear();
        // A lhs val cannot be contained in the closest subtree containing rhsVal
        void addRule(size_t lhsVal, size_t rhsVal1, size_t rhsVal2);

        const IndexedTopologyNode& getNode(size_t index) const;
        const std::vector<IndexedTopologyNode>& getNodes() const;
        bool canValBePlaced(size_t valNodeIndex, size_t otherNodeIndex) const;

        // ATTENTION: This can only be used to when moving a node within the same subtree
        // I.E siblings or lower
        bool canNodeBePlaced(size_t valNodeIndex, size_t otherNodeIndex) const;

        // Get which direct child of rootNode contains val
        size_t getSubtreeContainingVal (size_t rootNodeIndex, size_t val) const;
        std::set<size_t> getChildrenOf (size_t nodeIndex) const;

        size_t findClosestCommonAncestor(size_t nodeAIndex, size_t nodeBIndex) const;
        size_t findNode(size_t val) const;
        bool nodeExists(size_t val) const;
        size_t findParentNodeOf(size_t val) const;
        bool contains(size_t subtreeIndex, size_t val) const;

    private:
        std::vector<IndexedTopologyNode> nodes;
        //std::vector<std::pair<size_t, std::set<size_t>>> factList;
        std::map<std::set<size_t>, std::set<size_t>> factList;
    };

    std::ostream& operator<< (std::ostream& strm, const IndexedTopologyTree& indexedTopologyTree);
}


#endif //NETWORK_DISCOVERY_INDEXEDTOPOLOGYTREE_H
