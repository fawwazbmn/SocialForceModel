#ifndef WALL_H
#define WALL_H

#include <vecmath.h>

struct Line {
	Point3f start;
	Point3f end;
};

class Wall {
private:
	Line wall;

public:
	Wall();
	Wall(float x1, float y1, float x2, float y2);
	//~Wall();

	Point3f getStartPoint() const { return wall.start; }
	Point3f getEndPoint() const { return wall.end; }
	Point3f getNearestPoint(Point3f position_i);	// Computes distance between 'position_i' and wall
};

#endif