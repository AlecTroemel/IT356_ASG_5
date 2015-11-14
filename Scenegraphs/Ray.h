#ifndef RAY_H
#define RAY_H

using namespace std;
#include <glm/glm.hpp>

class Ray
{
private:

public:
	glm::vec4 start;
	glm::vec4 dir;

	Ray(int x, int y, int z)
	{
		this->start = glm::vec4(0, 0, -10, 0);
		this->dir = glm::vec4(x, y, z, 1);
	}

	~Ray() {};
};

#endif