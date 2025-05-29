#pragma once
#include <vector>
#include "../include/Body.h"
#include "../include/Constants.h"


// Helper functions for simulation utilities
double totEnergy(const std::vector<Body*>& bodies);
void sortByDistance(std::vector<Body*>& bodies);
void assignInitVelocities(std::vector<Body*>& bodies);



// Initial models 
void SolarSystem(std::vector<Body*>& bodies, int count);
void UniformDisk(std::vector<Body*>& bodies, int count);
void TwoGalaxySystem(std::vector<Body*>& bodies, int count);
