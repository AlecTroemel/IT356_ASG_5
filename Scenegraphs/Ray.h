#ifndef RAY_H
#define RAY_H

using namespace std;
#include <glm/glm.hpp>

class Ray
{
private:
	glm::vec4 start;
	glm::vec4 dir;
public:
	glm::vec4 getV()
	{
		return this->dir;
	}

	glm::vec4 getP()
	{
		return this->start;
	}

	void setP(glm::vec4 p)
	{
		this->start = p;
	}

	void setV(glm::vec4 v)
	{
		this->dir = v;
	}

	Ray()
	{
		this->start = glm::vec4(0, 0, 0, 1);
		this->dir = glm::vec4(0, 0, 0, 0);
	}

	Ray(int x, int y, int z)
	{
		this->start = glm::vec4(0, 0, 0, 1);
		this->dir = glm::vec4(x, y, z, 0);
	}

	~Ray() {};
};

#endif