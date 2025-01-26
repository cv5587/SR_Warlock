#pragma once

#include "Engine_Define.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <vector>

BEGIN(Engine)

/// Null node flag.
const unsigned int NULL_NODE = 0xffffffff;

class ENGINE_DLL AABB
{
public:
    AABB() :_surfaceArea(0.0)
    {

    }

    AABB(_vec3 lowerBound, _vec3 upperBound) :
        _lowerBound(lowerBound), _upperBound(upperBound)
    {
        // lowerbound가 upperBound보다 큰지 검사
        if (lowerBound.x > upperBound.x || lowerBound.y > upperBound.y || lowerBound.z > upperBound.z)
        {
            MSG_BOX("AABB lower bound is greater than the upper bound");
        }

        _surfaceArea = computeSurfaceArea();
        _centre = computeCentre();
    }


    ///표면적 계산
    double computeSurfaceArea() const;

    ///표면적 Get
    double getSurfaceArea() const;

    //두 aabb를 하나로 합침
    void merge(const AABB aabb1, const AABB aabb2);

    // aabb를 포함하는지 체크
    bool contains(const AABB aabb) const;

    // aabb와 겹치는지 체크
    bool overlaps(const AABB aabb, bool touchIsOverlap) const;

    // 중심 계산
    _vec3 computeCentre();

public:
    _vec3 _lowerBound;
    _vec3 _upperBound;

    // AABB 중심 위치
    _vec3 _centre;

    // AABB의 표면적
    _double _surfaceArea;
};

struct ENGINE_DLL Node
{
    Node()
    {

    }

    bool isLeaf() const
    {
        return (_left == NULL_NODE);
    }

    /// The fattened axis-aligned bounding box.
    AABB _aabb;

    /// Index of the parent node.
    unsigned int _parent;

    /// Index of the next node.
    unsigned int _next;

    /// Index of the left-hand child.
    unsigned int _left;

    /// Index of the right-hand child.
    unsigned int _right;

    /// Height of the node. This is 0 for a leaf and -1 for a free node.
    int _height;

    /// The index of the particle that the node contains (leaf nodes only).
    unsigned int _particle;

};

class ENGINE_DLL Tree
{
public:
    Tree(double skinThickness = 0.05, bool touchIsOverlap = true);
    Tree(double skinThickness, const std::vector<bool> periodicity, bool touchIsOverlap = true);


    void setPeriodicity(const std::vector<bool> periodicity);
    //void setBoxSize(const _vec3 boxSize);

    unsigned int allocateNode();
    void freeNode(unsigned int node);

    void insertParticle(const unsigned int particle, _vec3 position, double radius);
    void insertParticle(const unsigned int particle, _vec3 lowerBound, _vec3 upperBound);

    unsigned int nParticles();

    void removeParticle(unsigned int particle);

    /// Remove all particles from the tree.
    void removeAll();

    bool updateParticle(unsigned int particle, _vec3 position, double radius, bool alwaysReinsert = false);
    bool updateParticle(unsigned int particle, _vec3 lowerBound, _vec3 upperBound, bool alwaysReinsert = false);

    std::vector<unsigned int> query(unsigned int particle);
    std::vector<unsigned int> query(unsigned int particle, const AABB aabb);
    std::vector<unsigned int> query(const AABB aabb);

    unsigned int getHeight() const;
    unsigned int getNodeCount() const;
    unsigned int computeMaximumBalance() const;
    double computeSurfaceAreaRatio() const;

    /// Validate the tree.
    void validate() const;

    /// Rebuild an optimal tree.
    void rebuild();

private:
    /// The index of the root node.
    unsigned int _root;

    /// The dynamic tree.
    std::vector<Node> _nodes;

    unsigned int _nodeCount;

    unsigned int _nodeCapacity;

    /// The position of node at the top of the free list.
    unsigned int _freeList;
    /// Whether the system is periodic along at least one axis.
    bool _isPeriodic;
    /// The skin thickness of the fattened AABBs
    double _skinThickness;

    /// Whether the system is periodic along each axis.
    std::vector<bool> _periodicity;

    /// The size of the system in each dimension.

    /// A map between particle and node indices.
    std::unordered_map<unsigned int, unsigned int> _particleMap;

    /// Does touching count as overlapping in tree queries?
    bool _touchIsOverlap;



    void insertLeaf(unsigned int leaf);
    void removeLeaf(unsigned int leaf);

    unsigned int balance(unsigned int node);

    unsigned int computeHeight() const;

    unsigned int computeHeight(unsigned int node) const;

    void validateStructure(unsigned int node) const;
    void validateMetrics(unsigned int node) const;
};

END