#include <random>
#include "Agent.h"
using namespace std;

const float PI = 3.14159265359F;

int Agent::crowdIdx = -1;
default_random_engine generator;

Agent::Agent() {
	id = ++crowdIdx;
	
	radius = 0.2F;

	// Desired Speed Based on (Moussaid et al., 2009)
	normal_distribution<float> distribution(1.29F, 0.19F);	// Generate random value of mean 1.29 and standard deviation 0.19
	desiredSpeed = distribution(generator);
	
	colour.set(0.0, 0.0, 0.0);

	position.set(0.0, 0.0, 0.0);
	velocity.set(0.0, 0.0, 0.0);
}

Agent::~Agent() {
	path.clear();				// Remove waypoints
	crowdIdx--;
}

void Agent::setRadius(float radius) {
	this->radius = radius;
}

void Agent::setDesiredSpeed(float speed) {
	desiredSpeed = speed;
}

void Agent::setColour(float red, float green, float blue) {
	colour.set(red, green, blue);
}

void Agent::setPosition(float x, float y) {
	position.set(x, y, 0.0);
}

void Agent::setPath(float x, float y, float radius) {
	path.push_back({ Point3f(x, y, 0.0), radius });
}

Point3f Agent::getPath() {
	Vector3f distanceCurr, distanceNext;

	distanceCurr = path[0].position - position;			// Distance to current waypoint

	if (path.size() > 2) {
		distanceNext = path[1].position - position;		// Distance to next waypoint

		// Set Next Waypoint as Current Waypoint if Next Waypoint is Nearer
		if (distanceNext.lengthSquared() < distanceCurr.lengthSquared()) {
			path.push_back(path.front());
			path.pop_front();

			distanceCurr = distanceNext;
		}
	}

	// Move Front Point to Back if Within Radius
	if (distanceCurr.lengthSquared() < (path.front().radius * path.front().radius)) {
		path.push_back(path.front());
		path.pop_front();
	}

	return path.front().position;
}

float Agent::getOrientation() {
	return (atan2(velocity.y, velocity.x) * (180 / PI));
}

Point3f Agent::getAheadVector() const {
	return (velocity + position);
}

void Agent::move(vector<Agent *> agents, vector<Wall *> walls, float stepTime) {
	Vector3f acceleration;

	// Compute Social Force
	acceleration = drivingForce(getPath()) + agentInteractForce(agents) + wallInteractForce(walls);

	// Compute New Velocity
	velocity = velocity + acceleration * stepTime;

	// Truncate Velocity if Exceed Maximum Speed (Magnitude)
	if (velocity.lengthSquared() > (desiredSpeed * desiredSpeed)) {
		velocity.normalize();
		velocity *= desiredSpeed;
	}

	// Compute New Position
	position = position + velocity * stepTime;
}

Vector3f Agent::drivingForce(const Point3f position_target) {
	const float T = 0.54F;	// Relaxation time based on (Moussaid et al., 2009)
	Vector3f e_i, f_i;

	// Compute Desired Direction
	// Formula: e_i = (position_target - position_i) / ||(position_target - position_i)||
	e_i = position_target - position;
	e_i.normalize();

	// Compute Driving Force
	// Formula: f_i = ((desiredSpeed * e_i) - velocity_i) / T
	f_i = ((desiredSpeed * e_i) - velocity) * (1 / T);

	return f_i;
}

Vector3f Agent::agentInteractForce(vector<Agent *> agents) {
	// Constant Values Based on (Moussaid et al., 2009)
	const float lambda = 2.0;	// Weight reflecting relative importance of velocity vector against position vector
	const float gamma = 0.35F;	// Speed interaction
	const float n_prime = 3.0;	// Angular interaction
	const float n = 2.0;		// Angular intaraction
	const float A = 4.5;		// Modal parameter A

	Vector3f distance_ij, e_ij, D_ij, t_ij, n_ij, f_ij;
	float B, theta, f_v, f_theta;
	int K;

	f_ij.set(0.0, 0.0, 0.0);

	for (const Agent *agent_j : agents) {
		// Do Not Compute Interaction Force to Itself
		if (agent_j->id != id) {
			// Compute Distance Between Agent j and i
			distance_ij = agent_j->position - position;

			// Skip Computation if Agents i and j are Too Far Away
			if (distance_ij.lengthSquared() > (2.0 * 2.0))
				continue;

			// Compute Direction of Agent j from i
			// Formula: e_ij = (position_j - position_i) / ||position_j - position_i||
			e_ij = distance_ij;
			e_ij.normalize();

			// Compute Interaction Vector Between Agent i and j
			// Formula: D = lambda * (velocity_i - velocity_j) + e_ij
			D_ij = lambda * (velocity - agent_j->velocity) + e_ij;

			// Compute Modal Parameter B
			// Formula: B = gamma * ||D_ij||
			B = gamma * D_ij.length();

			// Compute Interaction Direction
			// Formula: t_ij = D_ij / ||D_ij||
			t_ij = D_ij;
			t_ij.normalize();

			// Compute Angle Between Interaction Direction (t_ij) and Vector Pointing from Agent i to j (e_ij)
			theta = t_ij.angle(e_ij);

			// Compute Sign of Angle 'theta'
			// Formula: K = theta / |theta|
			K = (theta == 0) ? 0 : static_cast<int>(theta / abs(theta));

			// Compute Amount of Deceleration
			// Formula: f_v = -A * exp(-distance_ij / B - ((n_prime * B * theta) * (n_prime * B * theta)))
			f_v = -A * exp(-distance_ij.length() / B - ((n_prime * B * theta) * (n_prime * B * theta)));

			// Compute Amount of Directional Changes
			// Formula: f_theta = -A * K * exp(-distance_ij / B - ((n * B * theta) * (n * B * theta)))
			f_theta = -A * K * exp(-distance_ij.length() / B - ((n * B * theta) * (n * B * theta)));

			// Compute Normal Vector of Interaction Direction Oriented to the Left
			n_ij.set(-t_ij.y, t_ij.x, 0.0);

			// Compute Interaction Force
			// Formula: f_ij = f_v * t_ij + f_theta * n_ij
			f_ij += f_v * t_ij + f_theta * n_ij;
		}
	}

	return f_ij;
}

Vector3f Agent::wallInteractForce(vector<Wall *> walls) {
	//const float repulsionRange = 0.3F;	// Repulsion range based on (Moussaid et al., 2009)
	const int a = 3;
	const float b = 0.1F;

	Point3f nearestPoint;
	Vector3f vector_wi, minVector_wi;
	float distanceSquared, minDistanceSquared = INFINITY, d_w, f_iw;

	for (Wall *wall : walls) {
		nearestPoint = wall->getNearestPoint(position);
		vector_wi = position - nearestPoint;	// Vector from wall to agent i
		distanceSquared = vector_wi.lengthSquared();

		// Store Nearest Wall Distance
		if (distanceSquared < minDistanceSquared) {
			minDistanceSquared = distanceSquared;
			minVector_wi = vector_wi;
		}
	}

	d_w = sqrt(minDistanceSquared) - radius;	// Distance between wall and agent i

	// Compute Interaction Force
	// Formula: f_iw = a * exp(-d_w / b)
	f_iw = a * exp(-d_w / b);
	minVector_wi.normalize();

	return f_iw * minVector_wi;
}