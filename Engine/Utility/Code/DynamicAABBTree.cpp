#include "..\..\Header\DynamicAABBTree.h"

double AABB::computeSurfaceArea() const
{
    _vec3 d = _upperBound - _lowerBound;
    return 2.0 * (d.x * d.y + d.y * d.z + d.z * d.x);
}

double AABB::getSurfaceArea() const
{
    return _surfaceArea;
}

void AABB::merge(const AABB aabb1, const AABB aabb2)
{

    _lowerBound.x = min(aabb1._lowerBound.x, aabb2._lowerBound.x);
    _upperBound.x = max(aabb1._upperBound.x, aabb2._upperBound.x);

    _lowerBound.y = min(aabb1._lowerBound.y, aabb2._lowerBound.y);
    _upperBound.y = max(aabb1._upperBound.y, aabb2._upperBound.y);

    _lowerBound.z = min(aabb1._lowerBound.z, aabb2._lowerBound.z);
    _upperBound.z = max(aabb1._upperBound.z, aabb2._upperBound.z);

    _surfaceArea = computeSurfaceArea();
    _centre = computeCentre();
}

bool AABB::contains(const AABB aabb) const
{
    if (aabb._lowerBound.x < _lowerBound.x) return false;
    if (aabb._upperBound.x > _upperBound.x) return false;

    if (aabb._lowerBound.y < _lowerBound.y) return false;
    if (aabb._upperBound.y > _upperBound.y) return false;

    if (aabb._lowerBound.z < _lowerBound.z) return false;
    if (aabb._upperBound.z > _upperBound.z) return false;

    return true;
}

bool AABB::overlaps(const AABB aabb, bool touchIsOverlap) const
{
    if (touchIsOverlap)
    {
        return aabb._lowerBound.x <= _upperBound.x &&
            aabb._upperBound.x >= _lowerBound.x &&
            aabb._lowerBound.y <= _upperBound.y &&
            aabb._upperBound.y >= _lowerBound.y &&
            aabb._lowerBound.z <= _upperBound.z &&
            aabb._upperBound.z >= _lowerBound.z;
    }
    else
    {
        return aabb._lowerBound.x < _upperBound.x &&
            aabb._upperBound.x > _lowerBound.x &&
            aabb._lowerBound.y < _upperBound.y &&
            aabb._upperBound.y > _lowerBound.y &&
            aabb._lowerBound.z < _upperBound.z &&
            aabb._upperBound.z > _lowerBound.z;
    }

}

_vec3 AABB::computeCentre()
{
    _vec3 pos;

    pos.x = 0.5 * (_lowerBound.x + _upperBound.x);
    pos.y = 0.5 * (_lowerBound.y + _upperBound.y);
    pos.z = 0.5 * (_lowerBound.z + _upperBound.z);

    return pos;
}

Tree::Tree(double skinThickness, bool touchIsOverlap)
    :_skinThickness(skinThickness), _touchIsOverlap(touchIsOverlap)
{
    _periodicity.resize(3);
    std::fill(_periodicity.begin(), _periodicity.end(), false);

    _root = NULL_NODE;
    _nodeCount = 0;
    _nodeCapacity = 1000;
    _nodes.resize(_nodeCapacity);

    // Build a linked list for the list of free nodes.
    for (unsigned int i = 0; i < _nodeCapacity - 1; i++)
    {
        _nodes[i]._next = i + 1;
        _nodes[i]._height = -1;
    }
    _nodes[_nodeCapacity - 1]._next = NULL_NODE;
    _nodes[_nodeCapacity - 1]._height = -1;

    // Assign the index of the first free node.
    _freeList = 0;
}

Tree::Tree(double skinThickness, const std::vector<bool> periodicity, bool touchIsOverlap)
    :_skinThickness(skinThickness),
    _periodicity(periodicity),_touchIsOverlap(touchIsOverlap)
{
    if (periodicity.size() != 3)
    {
        MSG_BOX("Dimensionality mismatch");
    }

    // Initialise the tree.
    _root = NULL_NODE;
    _nodeCount = 0;
    _nodeCapacity = 1000;
    _nodes.resize(_nodeCapacity);

    // Build a linked list for the list of free nodes.
    for (unsigned int i = 0; i < _nodeCapacity - 1; i++)
    {
        _nodes[i]._next = i + 1;
        _nodes[i]._height = -1;
    }
    _nodes[_nodeCapacity - 1]._next = NULL_NODE;
    _nodes[_nodeCapacity - 1]._height = -1;

    // Assign the index of the first free node.
    _freeList = 0;

    // Check periodicity.
    _isPeriodic = false;
    for (unsigned int i = 0; i < 3; i++)
    {
        if (periodicity[i])
        {
            _isPeriodic = true;
        }
    }

}

void Tree::setPeriodicity(const std::vector<bool> periodicity)
{
    _periodicity = periodicity;
}

unsigned int Tree::allocateNode()
{
    if (_freeList == NULL_NODE)
    {
        assert(_nodeCount == _nodeCapacity);

         //The free list is empty. Rebuild a bigger pool.
        _nodeCapacity *= 2;
        _nodes.resize(_nodeCapacity);

        // Build a linked list for the list of free nodes.
        for (unsigned int i = _nodeCount; i < _nodeCapacity - 1; i++)
        {
            _nodes[i]._next = i + 1;
            _nodes[i]._height = -1;
        }
        _nodes[_nodeCapacity - 1]._next = NULL_NODE;
        _nodes[_nodeCapacity - 1]._height = -1;

        // Assign the index of the first free node.
        _freeList = _nodeCount;
    }

    // Peel a node off the free list.
    unsigned int node = _freeList;
    _freeList = _nodes[node]._next;
    _nodes[node]._parent = NULL_NODE;
    _nodes[node]._left = NULL_NODE;
    _nodes[node]._right = NULL_NODE;
    _nodes[node]._height = 0;
    _nodeCount++;

    return node;
}

void Tree::freeNode(unsigned int node)
{
    assert(node < _nodeCapacity);
    assert(0 < _nodeCount);

    _nodes[node]._next = _freeList;
    _nodes[node]._height = -1;
    _freeList = node;
    _nodeCount--;
}

void Tree::insertParticle(unsigned int particle, _vec3 position, double radius)
{
    // Make sure the particle doesn't already exist.
    if (_particleMap.count(particle) != 0)
    {
        throw std::invalid_argument("[ERROR]: Particle already exists in tree!");
    }

    unsigned int node = allocateNode();

    std::vector<double> size(3);

    // Compute the AABB limits.

    _nodes[node]._aabb._lowerBound.x = position.x - radius;
    _nodes[node]._aabb._upperBound.x = position.x + radius;
    size[0] = _nodes[node]._aabb._upperBound.x - _nodes[node]._aabb._lowerBound.x;

    _nodes[node]._aabb._lowerBound.y = position.y - radius;
    _nodes[node]._aabb._upperBound.y = position.y + radius;
    size[1] = _nodes[node]._aabb._upperBound.y - _nodes[node]._aabb._lowerBound.y;

    _nodes[node]._aabb._lowerBound.z = position.z - radius;
    _nodes[node]._aabb._upperBound.z = position.z + radius;
    size[2] = _nodes[node]._aabb._upperBound.z - _nodes[node]._aabb._lowerBound.z;


    _nodes[node]._aabb._lowerBound.x -= _skinThickness * size[0];
    _nodes[node]._aabb._upperBound.x += _skinThickness * size[0];

    _nodes[node]._aabb._lowerBound.y -= _skinThickness * size[1];
    _nodes[node]._aabb._upperBound.y += _skinThickness * size[1];

    _nodes[node]._aabb._lowerBound.z -= _skinThickness * size[2];
    _nodes[node]._aabb._upperBound.z += _skinThickness * size[2];

    _nodes[node]._aabb._surfaceArea = _nodes[node]._aabb.computeSurfaceArea();
    _nodes[node]._aabb._centre = _nodes[node]._aabb.computeCentre();

    // Zero the height.
    _nodes[node]._height = 0;

    // Insert a new leaf into the tree.
    insertLeaf(node);

    // Add the new particle to the map.
    _particleMap.insert(std::unordered_map<unsigned int, unsigned int>::value_type(particle, node));

    // Store the particle index.
    _nodes[node]._particle = particle;
}

void Tree::insertParticle(unsigned int particle, _vec3 lowerBound, _vec3 upperBound)
{
    // Make sure the particle doesn't already exist.
    if (_particleMap.count(particle) != 0)
    {
        throw std::invalid_argument("[ERROR]: Particle already exists in tree!");
    }

    // Allocate a new node for the particle.
    unsigned int node = allocateNode();

    // AABB size in each dimension.
    std::vector<double> size(3);


    // Validate the bound.
    if (lowerBound.x > upperBound.x)
    {
        throw std::invalid_argument("[ERROR]: AABB lower bound is greater than the upper bound!");
    }

    _nodes[node]._aabb._lowerBound.x = lowerBound.x;
    _nodes[node]._aabb._upperBound.x = upperBound.x;
    size[0] = upperBound.x - lowerBound.x;

    // Validate the bound.
    if (lowerBound.y > upperBound.y)
    {
        throw std::invalid_argument("[ERROR]: AABB lower bound is greater than the upper bound!");
    }

    _nodes[node]._aabb._lowerBound.y = lowerBound.y;
    _nodes[node]._aabb._upperBound.y = upperBound.y;
    size[1] = upperBound.y - lowerBound.y;

    // Validate the bound.
    if (lowerBound.z > upperBound.z)
    {
        throw std::invalid_argument("[ERROR]: AABB lower bound is greater than the upper bound!");
    }

    _nodes[node]._aabb._lowerBound.z = lowerBound.z;
    _nodes[node]._aabb._upperBound.z = upperBound.z;
    size[2] = upperBound.z - lowerBound.z;


    _nodes[node]._aabb._lowerBound.x -= _skinThickness * size[0];
    _nodes[node]._aabb._upperBound.x += _skinThickness * size[0];

    _nodes[node]._aabb._lowerBound.y -= _skinThickness * size[1];
    _nodes[node]._aabb._upperBound.y += _skinThickness * size[1];

    _nodes[node]._aabb._lowerBound.z -= _skinThickness * size[2];
    _nodes[node]._aabb._upperBound.z += _skinThickness * size[2];


    _nodes[node]._aabb._surfaceArea = _nodes[node]._aabb.computeSurfaceArea();
    _nodes[node]._aabb._centre = _nodes[node]._aabb.computeCentre();

    // Zero the height.
    _nodes[node]._height = 0;

    // Insert a new leaf into the tree.
    insertLeaf(node);

    // Add the new particle to the map.
    _particleMap.insert(std::unordered_map<unsigned int, unsigned int>::value_type(particle, node));

    // Store the particle index.
    _nodes[node]._particle = particle;
}

unsigned int Tree::nParticles()
{
    return _particleMap.size();
}

void Tree::removeParticle(unsigned int particle)
{
    // Map iterator.
    std::unordered_map<unsigned int, unsigned int>::iterator it;

    // Find the particle.
    it = _particleMap.find(particle);

    // The particle doesn't exist.
    if (it == _particleMap.end())
    {
        throw std::invalid_argument("[ERROR]: Invalid particle index!");
    }

    // Extract the node index.
    unsigned int node = it->second;

    // Erase the particle from the map.
    _particleMap.erase(it);

    assert(node < _nodeCapacity);
    assert(_nodes[node].isLeaf());

    removeLeaf(node);
    freeNode(node);
}

void Tree::removeAll()
{
    // Iterator pointing to the start of the particle map.
    std::unordered_map<unsigned int, unsigned int>::iterator it = _particleMap.begin();

    // Iterate over the map.
    while (it != _particleMap.end())
    {
        // Extract the node index.
        unsigned int node = it->second;

        assert(node < _nodeCapacity);
        assert(_nodes[node].isLeaf());

        removeLeaf(node);
        freeNode(node);

        it++;
    }

    // Clear the particle map.
    _particleMap.clear();
}

bool Tree::updateParticle(unsigned int particle, _vec3 position, double radius, bool alwaysReinsert)
{
    _vec3 lowerBound;
    _vec3 upperBound;

    lowerBound.x = position.x - radius;
    upperBound.x = position.x + radius;

    lowerBound.y = position.y - radius;
    upperBound.y = position.y + radius;

    lowerBound.z = position.z - radius;
    upperBound.z = position.z + radius;


    // Update the particle.
    return updateParticle(particle, lowerBound, upperBound, alwaysReinsert);
}

bool Tree::updateParticle(unsigned int particle, _vec3 lowerBound, _vec3 upperBound, bool alwaysReinsert)
{
    // Map iterator.
    std::unordered_map<unsigned int, unsigned int>::iterator it;

    // Find the particle.
    it = _particleMap.find(particle);

    // The particle doesn't exist.
    if (it == _particleMap.end())
    {
        return true;
    }

    // Extract the node index.
    unsigned int node = it->second;

    assert(node < _nodeCapacity);
    assert(_nodes[node].isLeaf());

    // AABB size in each dimension.
    std::vector<double> size(3);

    // Compute the AABB limits.
        // Validate the bound.
    if (lowerBound.x > upperBound.x)
    {
        throw std::invalid_argument("[ERROR]: AABB lower bound is greater than the upper bound!");
    }

    size[0] = upperBound.x - lowerBound.x;


    if (lowerBound.y > upperBound.y)
    {
        throw std::invalid_argument("[ERROR]: AABB lower bound is greater than the upper bound!");
    }

    size[1] = upperBound.y - lowerBound.y;


    if (lowerBound.z > upperBound.z)
    {
        throw std::invalid_argument("[ERROR]: AABB lower bound is greater than the upper bound!");
    }

    size[2] = upperBound.z - lowerBound.z;




    // Create the new AABB.
    AABB aabb(lowerBound, upperBound);

    // No need to update if the particle is still within its fattened AABB.
    if (!alwaysReinsert && _nodes[node]._aabb.contains(aabb)) return false;

    // Remove the current leaf.
    removeLeaf(node);

    aabb._lowerBound.x -= _skinThickness * size[0];
    aabb._upperBound.x += _skinThickness * size[0];

    aabb._lowerBound.y -= _skinThickness * size[1];
    aabb._upperBound.y += _skinThickness * size[1];

    aabb._lowerBound.z -= _skinThickness * size[2];
    aabb._upperBound.z += _skinThickness * size[2];

    // Assign the new AABB.
    _nodes[node]._aabb = aabb;

    // Update the surface area and centroid.
    _nodes[node]._aabb._surfaceArea = _nodes[node]._aabb.computeSurfaceArea();
    _nodes[node]._aabb._centre = _nodes[node]._aabb.computeCentre();

    // Insert a new leaf node.
    insertLeaf(node);

    return true;
}

std::vector<unsigned int> Tree::query(unsigned int particle)
{
    // Make sure that this is a valid particle.
    if (_particleMap.count(particle) == 0)
    {
        throw std::invalid_argument("[ERROR]: Invalid particle index!");
    }
    // Test overlap of particle AABB against all other particles.
    return query(particle, _nodes[_particleMap.find(particle)->second]._aabb);
}

std::vector<unsigned int> Tree::query(unsigned int particle, const AABB aabb)
{
    std::vector<unsigned int> stack;
    stack.reserve(1000);
    stack.push_back(_root);

    std::vector<unsigned int> particles;

    while (stack.size() > 0)
    {
        unsigned int node = stack.back();
        stack.pop_back();

        // Copy the AABB.
        AABB nodeAABB = _nodes[node]._aabb;

        if (node == NULL_NODE) continue;

            

        // Test for overlap between the AABBs.
        if (aabb.overlaps(nodeAABB, _touchIsOverlap))
        {
            // Check that we're at a leaf node.
            if (_nodes[node].isLeaf())
            {
                // Can't interact with itself.
                if (_nodes[node]._particle != particle)
                {
                    particles.push_back(_nodes[node]._particle);
                }
            }
            else
            {
                stack.push_back(_nodes[node]._left);
                stack.push_back(_nodes[node]._right);
            }
        }
    }


    return particles;
}

std::vector<unsigned int> Tree::query(const AABB aabb)
{
    // Make sure the tree isn't empty.
    if (_particleMap.size() == 0)
    {
        return std::vector<unsigned int>();
    }

    // Test overlap of AABB against all particles.
    return query(UINT_MAX, aabb);
}


unsigned int Tree::getHeight() const
{
    if (_root == NULL_NODE) return 0;
    return _nodes[_root]._height;
}

unsigned int Tree::getNodeCount() const
{
    return _nodeCount;
}

unsigned int Tree::computeMaximumBalance() const
{
    unsigned int maxBalance = 0;
    for (unsigned int i = 0; i < _nodeCapacity; i++)
    {
        if (_nodes[i]._height <= 1)
            continue;

        assert(_nodes[i].isLeaf() == false);

        unsigned int balance = std::abs(_nodes[_nodes[i]._left]._height - _nodes[_nodes[i]._right]._height);
        maxBalance = max(maxBalance, balance);
    }

    return maxBalance;
}

double Tree::computeSurfaceAreaRatio() const
{
    if (_root == NULL_NODE) return 0.0;

    double rootArea = _nodes[_root]._aabb.computeSurfaceArea();
    double totalArea = 0.0;

    for (unsigned int i = 0; i < _nodeCapacity; i++)
    {
        if (_nodes[i]._height < 0) continue;

        totalArea += _nodes[i]._aabb.computeSurfaceArea();
    }

    return totalArea / rootArea;
}

void Tree::validate() const
{
#ifndef NDEBUG
    validateStructure(_root);
    validateMetrics(_root);

    unsigned int freeCount = 0;
    unsigned int freeIndex = _freeList;

    while (freeIndex != NULL_NODE)
    {
        assert(freeIndex < _nodeCapacity);
        freeIndex = _nodes[freeIndex]._next;
        freeCount++;
    }

    assert(getHeight() == computeHeight());
    assert((_nodeCount + freeCount) == _nodeCapacity);
#endif
}

void Tree::rebuild()
{
    std::vector<unsigned int> nodeIndices(_nodeCount);
    unsigned int count = 0;

    for (unsigned int i = 0; i < _nodeCapacity; i++)
    {
        // Free node.
        if (_nodes[i]._height < 0) continue;

        if (_nodes[i].isLeaf())
        {
            _nodes[i]._parent = NULL_NODE;
            nodeIndices[count] = i;
            count++;
        }
        else freeNode(i);
    }

    while (count > 1)
    {
        double minCost = DBL_MAX;
        int iMin = -1, jMin = -1;

        for (unsigned int i = 0; i < count; i++)
        {
            AABB aabbi = _nodes[nodeIndices[i]]._aabb;

            for (unsigned int j = i + 1; j < count; j++)
            {
                AABB aabbj = _nodes[nodeIndices[j]]._aabb;
                AABB aabb;
                aabb.merge(aabbi, aabbj);
                double cost = aabb.getSurfaceArea();

                if (cost < minCost)
                {
                    iMin = i;
                    jMin = j;
                    minCost = cost;
                }
            }
        }

        unsigned int index1 = nodeIndices[iMin];
        unsigned int index2 = nodeIndices[jMin];

        unsigned int parent = allocateNode();
        _nodes[parent]._left = index1;
        _nodes[parent]._right = index2;
        _nodes[parent]._height = 1 + max(_nodes[index1]._height, _nodes[index2]._height);
        _nodes[parent]._aabb.merge(_nodes[index1]._aabb, _nodes[index2]._aabb);
        _nodes[parent]._parent = NULL_NODE;

        _nodes[index1]._parent = parent;
        _nodes[index2]._parent = parent;

        nodeIndices[jMin] = nodeIndices[count - 1];
        nodeIndices[iMin] = parent;
        count--;
    }

    _root = nodeIndices[0];

    validate();
}

void Tree::insertLeaf(unsigned int leaf)
{
    if (_root == NULL_NODE)
    {
        _root = leaf;
        _nodes[_root]._parent = NULL_NODE;
        return;
    }

    // Find the best sibling for the node.

    AABB leafAABB = _nodes[leaf]._aabb;
    unsigned int index = _root;

    while (!_nodes[index].isLeaf())
    {
        // Extract the children of the node.
        unsigned int left = _nodes[index]._left;
        unsigned int right = _nodes[index]._right;

        double surfaceArea = _nodes[index]._aabb.getSurfaceArea();

        AABB combinedAABB;
        combinedAABB.merge(_nodes[index]._aabb, leafAABB);
        double combinedSurfaceArea = combinedAABB.getSurfaceArea();

        // Cost of creating a new parent for this node and the new leaf.
        double cost = 2.0 * combinedSurfaceArea;

        // Minimum cost of pushing the leaf further down the tree.
        double inheritanceCost = 2.0 * (combinedSurfaceArea - surfaceArea);

        // Cost of descending to the left.
        double costLeft;
        if (_nodes[left].isLeaf())
        {
            AABB aabb;
            aabb.merge(leafAABB, _nodes[left]._aabb);
            costLeft = aabb.getSurfaceArea() + inheritanceCost;
        }
        else
        {
            AABB aabb;
            aabb.merge(leafAABB, _nodes[left]._aabb);
            double oldArea = _nodes[left]._aabb.getSurfaceArea();
            double newArea = aabb.getSurfaceArea();
            costLeft = (newArea - oldArea) + inheritanceCost;
        }

        // Cost of descending to the right.
        double costRight;
        if (_nodes[right].isLeaf())
        {
            AABB aabb;
            aabb.merge(leafAABB, _nodes[right]._aabb);
            costRight = aabb.getSurfaceArea() + inheritanceCost;
        }
        else
        {
            AABB aabb;
            aabb.merge(leafAABB, _nodes[right]._aabb);
            double oldArea = _nodes[right]._aabb.getSurfaceArea();
            double newArea = aabb.getSurfaceArea();
            costRight = (newArea - oldArea) + inheritanceCost;
        }

        // Descend according to the minimum cost.
        if ((cost < costLeft) && (cost < costRight)) break;

        // Descend.
        if (costLeft < costRight) index = left;
        else                      index = right;
    }

    unsigned int sibling = index;

    // Create a new parent.
    unsigned int oldParent = _nodes[sibling]._parent;
    unsigned int newParent = allocateNode();
    _nodes[newParent]._parent = oldParent;
    _nodes[newParent]._aabb.merge(leafAABB, _nodes[sibling]._aabb);
    _nodes[newParent]._height = _nodes[sibling]._height + 1;

    // The sibling was not the root.
    if (oldParent != NULL_NODE)
    {
        if (_nodes[oldParent]._left == sibling) _nodes[oldParent]._left = newParent;
        else                                  _nodes[oldParent]._right = newParent;

        _nodes[newParent]._left = sibling;
        _nodes[newParent]._right = leaf;
        _nodes[sibling]._parent = newParent;
        _nodes[leaf]._parent = newParent;
    }
    // The sibling was the root.
    else
    {
        _nodes[newParent]._left = sibling;
        _nodes[newParent]._right = leaf;
        _nodes[sibling]._parent = newParent;
        _nodes[leaf]._parent = newParent;
        _root = newParent;
    }

    // Walk back up the tree fixing heights and AABBs.
    index = _nodes[leaf]._parent;
    while (index != NULL_NODE)
    {
        index = balance(index);

        unsigned int left = _nodes[index]._left;
        unsigned int right = _nodes[index]._right;

        assert(left != NULL_NODE);
        assert(right != NULL_NODE);

        _nodes[index]._height = 1 + max(_nodes[left]._height, _nodes[right]._height);
        _nodes[index]._aabb.merge(_nodes[left]._aabb, _nodes[right]._aabb);

        index = _nodes[index]._parent;
    }
}

void Tree::removeLeaf(unsigned int leaf)
{
    if (leaf == _root)
    {
        _root = NULL_NODE;
        return;
    }

    unsigned int parent = _nodes[leaf]._parent;
    unsigned int grandParent = _nodes[parent]._parent;
    unsigned int sibling;

    if (_nodes[parent]._left == leaf) sibling = _nodes[parent]._right;
    else                            sibling = _nodes[parent]._left;

    // Destroy the parent and connect the sibling to the grandparent.
    if (grandParent != NULL_NODE)
    {
        if (_nodes[grandParent]._left == parent) _nodes[grandParent]._left = sibling;
        else                                   _nodes[grandParent]._right = sibling;

        _nodes[sibling]._parent = grandParent;
        freeNode(parent);

        // Adjust ancestor bounds.
        unsigned int index = grandParent;
        while (index != NULL_NODE)
        {
            index = balance(index);

            unsigned int left = _nodes[index]._left;
            unsigned int right = _nodes[index]._right;

            _nodes[index]._aabb.merge(_nodes[left]._aabb, _nodes[right]._aabb);
            _nodes[index]._height = 1 + max(_nodes[left]._height, _nodes[right]._height);

            index = _nodes[index]._parent;
        }
    }
    else
    {
        _root = sibling;
        _nodes[sibling]._parent = NULL_NODE;
        freeNode(parent);
    }
}

unsigned int Tree::balance(unsigned int node)
{
    assert(node != NULL_NODE);

    if (_nodes[node].isLeaf() || (_nodes[node]._height < 2))
        return node;

    unsigned int left = _nodes[node]._left;
    unsigned int right = _nodes[node]._right;

    assert(left < _nodeCapacity);
    assert(right < _nodeCapacity);

    int currentBalance = _nodes[right]._height - _nodes[left]._height;

    // Rotate right branch up.
    if (currentBalance > 1)
    {
        unsigned int rightLeft = _nodes[right]._left;
        unsigned int rightRight = _nodes[right]._right;

        assert(rightLeft < _nodeCapacity);
        assert(rightRight < _nodeCapacity);

        // Swap node and its right-hand child.
        _nodes[right]._left = node;
        _nodes[right]._parent = _nodes[node]._parent;
        _nodes[node]._parent = right;

        // The node's old parent should now point to its right-hand child.
        if (_nodes[right]._parent != NULL_NODE)
        {
            if (_nodes[_nodes[right]._parent]._left == node) _nodes[_nodes[right]._parent]._left = right;
            else
            {
                assert(_nodes[_nodes[right]._parent]._right == node);
                _nodes[_nodes[right]._parent]._right = right;
            }
        }
        else _root = right;

        // Rotate.
        if (_nodes[rightLeft]._height > _nodes[rightRight]._height)
        {
            _nodes[right]._right = rightLeft;
            _nodes[node]._right = rightRight;
            _nodes[rightRight]._parent = node;
            _nodes[node]._aabb.merge(_nodes[left]._aabb, _nodes[rightRight]._aabb);
            _nodes[right]._aabb.merge(_nodes[node]._aabb, _nodes[rightLeft]._aabb);

            _nodes[node]._height = 1 + max(_nodes[left]._height, _nodes[rightRight]._height);
            _nodes[right]._height = 1 + max(_nodes[node]._height, _nodes[rightLeft]._height);
        }
        else
        {
            _nodes[right]._right = rightRight;
            _nodes[node]._right = rightLeft;
            _nodes[rightLeft]._parent = node;
            _nodes[node]._aabb.merge(_nodes[left]._aabb, _nodes[rightLeft]._aabb);
            _nodes[right]._aabb.merge(_nodes[node]._aabb, _nodes[rightRight]._aabb);

            _nodes[node]._height = 1 + max(_nodes[left]._height, _nodes[rightLeft]._height);
            _nodes[right]._height = 1 + max(_nodes[node]._height, _nodes[rightRight]._height);
        }

        return right;
    }

    // Rotate left branch up.
    if (currentBalance < -1)
    {
        unsigned int leftLeft = _nodes[left]._left;
        unsigned int leftRight = _nodes[left]._right;

        assert(leftLeft < _nodeCapacity);
        assert(leftRight < _nodeCapacity);

        // Swap node and its left-hand child.
        _nodes[left]._left = node;
        _nodes[left]._parent = _nodes[node]._parent;
        _nodes[node]._parent = left;

        // The node's old parent should now point to its left-hand child.
        if (_nodes[left]._parent != NULL_NODE)
        {
            if (_nodes[_nodes[left]._parent]._left == node) _nodes[_nodes[left]._parent]._left = left;
            else
            {
                assert(_nodes[_nodes[left]._parent]._right == node);
                _nodes[_nodes[left]._parent]._right = left;
            }
        }
        else _root = left;

        // Rotate.
        if (_nodes[leftLeft]._height > _nodes[leftRight]._height)
        {
            _nodes[left]._right = leftLeft;
            _nodes[node]._left = leftRight;
            _nodes[leftRight]._parent = node;
            _nodes[node]._aabb.merge(_nodes[right]._aabb, _nodes[leftRight]._aabb);
            _nodes[left]._aabb.merge(_nodes[node]._aabb, _nodes[leftLeft]._aabb);

            _nodes[node]._height = 1 + max(_nodes[right]._height, _nodes[leftRight]._height);
            _nodes[left]._height = 1 + max(_nodes[node]._height, _nodes[leftLeft]._height);
        }
        else
        {
            _nodes[left]._right = leftRight;
            _nodes[node]._left = leftLeft;
            _nodes[leftLeft]._parent = node;
            _nodes[node]._aabb.merge(_nodes[right]._aabb, _nodes[leftLeft]._aabb);
            _nodes[left]._aabb.merge(_nodes[node]._aabb, _nodes[leftRight]._aabb);

            _nodes[node]._height = 1 + max(_nodes[right]._height, _nodes[leftLeft]._height);
            _nodes[left]._height = 1 + max(_nodes[node]._height, _nodes[leftRight]._height);
        }

        return left;
    }

    return node;
}

unsigned int Tree::computeHeight() const
{
    return computeHeight(_root);
}

unsigned int Tree::computeHeight(unsigned int node) const
{
    assert(node < _nodeCapacity);

    if (_nodes[node].isLeaf()) return 0;

    unsigned int height1 = computeHeight(_nodes[node]._left);
    unsigned int height2 = computeHeight(_nodes[node]._right);

    return 1 + max(height1, height2);
}

void Tree::validateStructure(unsigned int node) const
{
    if (node == NULL_NODE) return;

    if (node == _root) assert(_nodes[node]._parent == NULL_NODE);

    unsigned int left = _nodes[node]._left;
    unsigned int right = _nodes[node]._right;

    if (_nodes[node].isLeaf())
    {
        assert(left == NULL_NODE);
        assert(right == NULL_NODE);
        assert(_nodes[node]._height == 0);
        return;
    }

    assert(left < _nodeCapacity);
    assert(right < _nodeCapacity);

    assert(_nodes[left]._parent == node);
    assert(_nodes[right]._parent == node);

    validateStructure(left);
    validateStructure(right);
}

void Tree::validateMetrics(unsigned int node) const
{
    if (node == NULL_NODE) return;

    unsigned int left = _nodes[node]._left;
    unsigned int right = _nodes[node]._right;

    if (_nodes[node].isLeaf())
    {
        assert(left == NULL_NODE);
        assert(right == NULL_NODE);
        assert(_nodes[node]._height == 0);
        return;
    }

    assert(left < _nodeCapacity);
    assert(right < _nodeCapacity);

    int height1 = _nodes[left]._height;
    int height2 = _nodes[right]._height;
    int height = 1 + max(height1, height2);
    (void)height; // Unused variable in Release build
    assert(_nodes[node]._height == height);

    AABB aabb;
    aabb.merge(_nodes[left]._aabb, _nodes[right]._aabb);


    assert(aabb._lowerBound.x == _nodes[node]._aabb._lowerBound.x);
    assert(aabb._upperBound.x == _nodes[node]._aabb._upperBound.x);

    assert(aabb._lowerBound.y == _nodes[node]._aabb._lowerBound.y);
    assert(aabb._upperBound.y == _nodes[node]._aabb._upperBound.y);

    assert(aabb._lowerBound.z == _nodes[node]._aabb._lowerBound.z);
    assert(aabb._upperBound.z == _nodes[node]._aabb._upperBound.z);

    validateMetrics(left);
    validateMetrics(right);
}

