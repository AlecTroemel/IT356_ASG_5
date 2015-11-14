#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <stack>
#include <glm/glm.hpp>
#include <vector>
#include <utils/Material.h>
#include <utils/Light.h>
#include <utils/Texture.h>

using namespace std;

class Raytracer
{
private:
	class Ray
	{
	private:
		glm::vec3 start;
		glm::vec3 dir;
	public:
		Ray(int x, int y, int z) 
		{
			this->start = glm::vec3(0,0,0);
			this->dir = glm::vec3(x, y, z);
		}

		~Ray() {};
	};
	
	class Hitrecord
	{
	private:
		float t;
		glm::vec3 hitPoint;
		glm::vec3 normal;
		graphics::Material material;
		glm::vec2 textureCoord;
		graphics::Texture texture;

	public:
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

	// the image as (R,G,B)
	vector<glm::vec4> pixels;
	vector<graphics::Light> lights;


public:
	Raytracer() 
	{
	}

	~Raytracer() 
	{
	}

	vector<glm::vec4> Raytrace(const int width, const int height)
	{
		glm::mat4 modelview;

		// load world to view in modelview

		// do the casting stuuuufffffff
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// create Ray from camera through point (x,y)
				Ray R(x - (0.5 * width), 
					  y - (0.5 * height), 
					  -(0.5 * height) / tan(100.0f*3.14159f / 90));
				
				glm::vec4 color = Raycast(R, modelview);
				this->pixels.push_back(color);
			}
		}

		// return  the image as a 1d array of vec3
		return pixels;
	}


	glm::vec4 Raycast(Ray R, glm::mat4 modelview)
	{

		// load world to view in modelview

		Hitrecord hr;
		graphics::Material* mat_pointer = closest_intersection(R, modelview, hr);

		if (mat_pointer == NULL) 
		{
			return glm::vec4(0, 0, 0, 1);
		}
		else
		{
			glm::vec4 color;
			// color = shade(R, hr, mat_pointer); // for lighting
			color = glm::vec4(1, 1, 1, 1);
			return color;
		}
	}

	graphics::Material* closest_intersection(Ray R, glm::mat4 modelview, Hitrecord hitrecord)
	{
		return NULL;
	}

	glm::vec4 shade(Ray R, Hitrecord hr, graphics::Material* mat_pointer)
	{

	}


};

#endif