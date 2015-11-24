#ifndef HITRECORD_H
#define HITRECORD_H

using namespace std;
#include <glm/glm.hpp>
#include "utils\Material.h"
#include "utils\Texture.h"

class Hitrecord
{
private:
	float t;
	glm::vec4 hitPoint;
	glm::vec4 normal;
	graphics::Material material;
	glm::vec2 textureCoord;
	graphics::Texture * texture;
public:
	Hitrecord()
	{
		this->t = FLT_MAX;
	}

	~Hitrecord()
	{
	}

	// getters and setters 
	// T
	void setT(float t)
	{
		this->t = t;
	}
	float getT()
	{
		return this->t;
	}

	//hitPoint (in view Coord system)
	void setHitPoint(glm::vec4 hp)
	{
		this->hitPoint = hp;
	}
	glm::vec4 getHitPoint()
	{
		return this->hitPoint;
	}

	// normal (in view Coord system)
	void setNormal(glm::vec4 n)
	{
		this->normal = n;
	}
	glm::vec4 getNormal()
	{
		return this->normal;
	}

	// material 
	void setMaterial(graphics::Material m)
	{
		this->material = m;
	}
	graphics::Material getMaterial()
	{
		return this->material;
	}

	//texture coordinates
	void setTextureCoords(glm::vec2 tc)
	{
		this->textureCoord = tc;
	}
	glm::vec2 getTextureCoord()
	{
		return this->textureCoord;
	}

	//texture 
	void setTexture(graphics::Texture *t)
	{
		this->texture = t;
	}
	graphics::Texture* getTexture()
	{
		return this->texture;
	}
};

#endif