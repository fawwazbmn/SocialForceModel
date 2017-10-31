#include "Wall.h"

Wall::Wall() {
	wall.start.set(0.0, 0.0, 0.0);
	wall.end.set(0.0, 0.0, 0.0);
}

Wall::Wall(float x1, float y1, float x2, float y2) {
	wall.start.set(x1, y1, 0.0);
	wall.end.set(x2, y2, 0.0);
}

Point3f Wall::getNearestPoint(Point3f position_i) {
	Vector3f relativeEnd, relativePos, relativeEndScal, relativePosScal;
	float dotProduct;
	Point3f nearestPoint;

	// Create Vector Relative to Wall's 'start'
	relativeEnd = wall.end - wall.start;	// Vector from wall's 'start' to 'end'
	relativePos = position_i - wall.start;	// Vector from wall's 'start' to agent i 'position'

	// Scale Both Vectors by the Length of the Wall
	relativeEndScal = relativeEnd;
	relativeEndScal.normalize();

	relativePosScal = relativePos * (1.0F / relativeEnd.length());

	// Compute Dot Product of Scaled Vectors
	dotProduct = relativeEndScal.dot(relativePosScal);

	if (dotProduct < 0.0)		// Position of Agent i located before wall's 'start'
		nearestPoint = wall.start;
	else if (dotProduct > 1.0)	// Position of Agent i located after wall's 'end'
		nearestPoint = wall.end;
	else						// Position of Agent i located between wall's 'start' and 'end'
		nearestPoint = (relativeEnd * dotProduct) + wall.start;

	return nearestPoint;
}