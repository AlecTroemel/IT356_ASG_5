#include "Scenegraph.h"
#include <stack>
#include "TransformNode.h"
#include <string>
#include <sstream>

using namespace std;

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Scenegraph::Scenegraph()
{
    root = NULL;
}

void Scenegraph::makeScenegraph(Node *r)
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
    this->root = r;
	if (this->root!=NULL)
		this->root->setScenegraph(this);

	createNodes();
}


Scenegraph::~Scenegraph()
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }

}

void Scenegraph::initShaderProgram(GLint shaderProgram)
{
   // lighting 
   projectionLocation = glGetUniformLocation(shaderProgram, "projection");
   if (projectionLocation < 0) cout << "projectionLocation not found ..." << endl;
   modelviewLocation = glGetUniformLocation(shaderProgram, "modelview");
   if (modelviewLocation < 0) cout << "modelviewLocation not found ..." << endl;
   normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalmatrix");
   if (modelviewLocation < 0) cout << "modelviewLocation not found ..." << endl;
   numLightsLocation = glGetUniformLocation(shaderProgram, "numLights");
   if (numLightsLocation < 0) cout << "numLightsLocation not found ..." << endl;

   // Material
   mat_ambientLocation = glGetUniformLocation(shaderProgram, "material.ambient");
   if (mat_ambientLocation < 0) cout << "mat_ambientLocation not found ..." << endl;
   mat_diffuseLocation = glGetUniformLocation(shaderProgram, "material.diffuse");
   if (mat_diffuseLocation < 0) cout << "mat_diffuseLocation not found ..." << endl;
   mat_specularLocation = glGetUniformLocation(shaderProgram, "material.specular");
   if (mat_specularLocation < 0) cout << "mat_specularLocation not found ..." << endl;
   mat_shininessLocation = glGetUniformLocation(shaderProgram, "material.shininess");
   if (mat_shininessLocation < 0) cout << "mat_shininessLocation not found ..." << endl;

   // Textures
   texturematrixLocation = glGetUniformLocation(shaderProgram, "texturematrix");
   if (texturematrixLocation < 0) cout << "texturematrixLocation not found ..." << endl;
   textureLocation = glGetUniformLocation(shaderProgram, "image");
   if (textureLocation < 0) cout << "textureLocation not found ..." << endl;

   // this is the problem
   for (int i = 0; i < 10; i++)
   {
	   stringstream name;

	   name << "light[" << i << "].ambient";

	   lightLocation[i].ambientLocation = glGetUniformLocation(shaderProgram, name.str().c_str());
	   if (lightLocation[i].ambientLocation < 0) cout << "lightLocation[" << i <<"].ambientLocation not found ..." << endl;


	   name.clear();//clear any bits set
	   name.str(std::string());

	   name << "light[" << i << "].diffuse";

	   lightLocation[i].diffuseLocation = glGetUniformLocation(shaderProgram, name.str().c_str());
	   if (lightLocation[i].diffuseLocation < 0) cout << "lightLocation[i].diffuseLocation not found ..." << endl;


	   name.clear();//clear any bits set
	   name.str(std::string());

	   name << "light[" << i << "].specular";

	   lightLocation[i].specularLocation = glGetUniformLocation(shaderProgram, name.str().c_str());
	   if (lightLocation[i].specularLocation < 0) cout << "lightLocation[i].specularLocation not found ..." << endl;


	   name.clear();//clear any bits set
	   name.str(std::string());

	   name << "light[" << i << "].position";

	   lightLocation[i].positionLocation = glGetUniformLocation(shaderProgram, name.str().c_str());
	   if (lightLocation[i].positionLocation < 0) cout << "lightLocation[i].positionLocation not found ..." << endl;


	   name.clear();//clear any bits set
	   name.str(std::string());
   }


   // set up the default white texture
	string defaultTextureName = "textures/white.png";
	deafultTexture.createImage(defaultTextureName);
}

void Scenegraph::draw(stack<glm::mat4>& modelView)
{
	vector<graphics::Light*> * lightsHolder = lightsToViewCoord(modelView); //new vector<graphics::Light*>; // 
	glUniform1i(numLightsLocation, 3);

	//light properties 
	for (int i = 0; i < lightsHolder->size(); i++)
	{
		if (debug) {
			cout << i
				<< " " << lightsHolder->at(i)->getPosition().x
				<< " " << lightsHolder->at(i)->getPosition().y
				<< " " << lightsHolder->at(i)->getPosition().z << endl;
		}
		glUniform3fv(lightLocation[i].ambientLocation, 1, glm::value_ptr(lightsHolder->at(i)->getAmbient()));
		glUniform3fv(lightLocation[i].diffuseLocation, 1, glm::value_ptr(lightsHolder->at(i)->getDiffuse()));
		glUniform3fv(lightLocation[i].specularLocation, 1, glm::value_ptr(lightsHolder->at(i)->getSpecular()));
		glUniform4fv(lightLocation[i].positionLocation, 1, glm::value_ptr(lightsHolder->at(i)->getPosition()));
	}
	debug = false;
	delete lightsHolder; // clean up the lightsHolder 

    if (root!=NULL)
    {
        root->draw(modelView);
    }
}




void Scenegraph::animate(float time)
{
	
}

void Scenegraph::createNodes()
{
	
}

vector<graphics::Light*> * Scenegraph::lightsToViewCoord(stack<glm::mat4>& modelView)
{
	// fix this without pointers

	// regernerate the maze woth material 
	vector<graphics::Light*> * lightsHolder = new vector<graphics::Light*>();

	if (root != NULL)
	{
		root->getLights(modelView, lightsHolder);
	}

	return lightsHolder;
}


/***************************** Raytracing Stuff *****************************/

float Scenegraph::Raytrace(const int width, const int height, stack<glm::mat4>& modelView)
{

	const int size = width * height * 3;
	float *pixels = new float[size];

	// load world to view in modelview

	// do the casting stuuuufffffff
	int counter = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// create Ray from camera through point (x,y)
			Ray R(x - (0.5 * width),
				y - (0.5 * height),
				-(0.5 * height) / tan(100.0f*3.14159f / 90));

			glm::vec4 color;
			Raycast(R, modelView, color);

			pixels[counter] = color.x;
			counter++;
			pixels[counter] = color.y;	
			counter++;
			pixels[counter] = color.z;
			counter++;

		}
	}

	// return  the image as a 1d array of vec3
	return *pixels;
}

bool Scenegraph::Raycast(Ray R, stack<glm::mat4>& modelView, glm::vec4 &color)
{
	Hitrecord hr;

	bool hit = root->intersect(R, hr, modelView);

	if (hit == true) 
	{
		color = glm::vec4(1, 1, 1, 1);
	}
	else // did not hit anything, so return background color
	{
		color = glm::vec4(.2, .2, .2, 1);
	}

	return hit;

}

