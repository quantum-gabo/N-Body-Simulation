#pragma once
#include "../include/Body.h"
#include "../include/Quadtree.h"
#include <vector>

class Forces {
public:
    // Direct O(n²) force calculation between all pairs of bodies
    static void calculateDirectForces(std::vector<Body*>& bodies);
    
    // Barnes-Hut O(n log n) force calculation using quadtree
    static void calculateBarnesHutForces(std::vector<Body*>& bodies,Quadtree& tree, float theta, float epsilon);
};
