#ifndef AGENT_H
#define AGENT_H

#include <vecmath.h>
#include <deque>
#include <vector>
#include "Wall.h"

struct Waypoint {
	Point3f position;
	float radius;
};

class Agent {
private:
	static int crowdIdx;	// Keep track of 'crowd' vector index in 'SocialForce.h'

	int id;
	float radius;
	float desiredSpeed;
	Color3f colour;

	Point3f position;
	std::deque<Waypoint> path;
	Vector3f velocity;

	Vector3f drivingForce(const Point3f position_target);		// Computes f_i
	Vector3f agentInteractForce(std::vector<Agent *> agents);	// Computes f_ij
	Vector3f wallInteractForce(std::vector<Wall *> walls);		// Computes f_iw

public:
	Agent();
	~Agent();

	void setRadius(float radius);
	void setDesiredSpeed(float speed);
	void setColour(float red, float green, float blue);
	void setPosition(float x, float y);
	void setPath(float x, float y, float radius);

	int getId() const { return id; }
	float getRadius() const { return radius; }
	float getDesiredSpeed() const { return desiredSpeed; }
	Color3f getColour() const { return colour; }
	Point3f getPosition() const { return position; }
	Point3f getPath();
	Vector3f getVelocity() const { return velocity; }
	float getOrientation();
	Point3f getAheadVector() const;

	void move(std::vector<Agent *> agents, std::vector<Wall *> walls, float stepTime);
};

#endif