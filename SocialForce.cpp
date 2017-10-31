#include "SocialForce.h"
using namespace std;

SocialForce::~SocialForce() {
	removeCrowd();
	removeWalls();
}

void SocialForce::addAgent(Agent *agent) {
	crowd.push_back(agent);
}

void SocialForce::addWall(Wall *wall) {
	walls.push_back(wall);
}

void SocialForce::removeAgent() {
	int lastIdx;

	if (!crowd.empty()) {
		lastIdx = crowd.size() - 1;		// Assign index of last element

		delete crowd[lastIdx];
		crowd.pop_back();
	}
}

void SocialForce::removeCrowd() {
	for (unsigned int idx = 0; idx < crowd.size(); idx++)
		delete crowd[idx];

	crowd.clear();
}

void SocialForce::removeWalls() {
	for (unsigned int idx = 0; idx < walls.size(); idx++)
		delete walls[idx];

	walls.clear();
}

void SocialForce::moveCrowd(float stepTime) {
	for (unsigned int idx = 0; idx < crowd.size(); idx++)
		crowd[idx]->move(crowd, walls, stepTime);
}