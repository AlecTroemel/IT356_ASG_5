#ifndef RAY_H
#define RAY_H

using namespace std;
#include <glm/glm.hpp>

class Ray
{
private:
	glm::vec3 start;
	glm::vec3 dir;
public:
	Ray(int x, int y, int z)
	{
		this->start = glm::vec3(0, 0, 0);
		this->dir = glm::vec3(x, y, z);
	}

	~Ray() {};
};

#endif