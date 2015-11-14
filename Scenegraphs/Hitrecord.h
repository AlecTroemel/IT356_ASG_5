#ifndef HITRECORD_H
#define HITRECORD_H

using namespace std;
#include <glm/glm.hpp>
#include "utils\Material.h"
#include "utils\Texture.h"

class Hitrecord
{
private:


public:
	float t;
	glm::vec3 hitPoint;
	glm::vec3 normal;
	graphics::Material material;
	glm::vec2 textureCoord;
	graphics::Texture texture;


	Hitrecord()
	{

	}

	Hitrecord(float t, glm::vec3 hp, glm::vec3 n, graphics::Material m)
	{
		this->t = t;
		this->hitPoint = hp;
		this->normal = n;
		this->material = m;
	}

	~Hitrecord()
	{
	}


};