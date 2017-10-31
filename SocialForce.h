#ifndef SOCIAL_FORCE_H
#define SOCIAL_FORCE_H

#include <vector>
#include "Agent.h"
#include "Wall.h"

class SocialForce {
private:
	std::vector<Agent *> crowd;
	std::vector<Wall *> walls;

public:
	//SocialForce();
	~SocialForce();

	void addAgent(Agent *agent);
	void addWall(Wall *wall);

	const std::vector<Agent *> getCrowd() const { return crowd; }
	int getCrowdSize() const { return crowd.size(); }
	const std::vector<Wall *> getWalls() const { return walls; }
	int getNumWalls() const { return walls.size(); }

	void removeAgent();		// Removes individual or single group
	void removeCrowd();		// Remove all individuals and groups
	void removeWalls();
	void moveCrowd(float stepTime);
};

#endif