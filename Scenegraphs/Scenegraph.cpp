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

// for player movement
void Scenegraph::movePlayer(float playerX, float playerY)
{
	dynamic_cast<TransformNode*> (player)->setAnimationTransform(
		glm::translate(glm::mat4(1.0), glm::vec3((270 - playerY), (225.5 - playerX), 0)));
}

void Scenegraph::moveCheckpoint()
{
	if (checkPointCounter == 0)
	{
		dynamic_cast<TransformNode*> (checkPoint1)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -3))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 0.01)));

		dynamic_cast<TransformNode*> (checkPoint2)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 990))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 200)));
	}
	if (checkPointCounter == 1)
	{
		dynamic_cast<TransformNode*> (checkPoint2)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -3))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 0.01)));

		dynamic_cast<TransformNode*> (checkPoint3)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 990))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 200)));
	}
	if (checkPointCounter == 2)
	{
		dynamic_cast<TransformNode*> (checkPoint3)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -3))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 0.01)));

		dynamic_cast<TransformNode*> (checkPoint4)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 990))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 200)));
	}
	if (checkPointCounter == 3)
	{
		dynamic_cast<TransformNode*> (checkPoint4)->setAnimationTransform(
			glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -4))
			*glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 0.01)));
	}

	checkPointCounter++;
}




void Scenegraph::animate(float time)
{
	
	// Brad Animation 
	dynamic_cast<TransformNode*> (arm1)->setAnimationTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(0, 10, 0))
		*glm::rotate(glm::mat4(1.0), sin(time) / 2, glm::vec3(1.0f, 0.0f, 0.0f))
		*glm::translate(glm::mat4(1.0), glm::vec3(0, -10, 0)));

	dynamic_cast<TransformNode*> (arm2)->setAnimationTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(0, 10, 0))
		*glm::rotate(glm::mat4(1.0), cos(time) / 2, glm::vec3(0.0f, 0.0f, 1.0f))
		*glm::translate(glm::mat4(1.0), glm::vec3(0, -10, 0)));

	dynamic_cast<TransformNode*> (arm3)->setAnimationTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(0, 10, 0))
		*glm::rotate(glm::mat4(1.0), sin(time) / 2, glm::vec3(1.0f, 0.0f, 0.0f))
		*glm::translate(glm::mat4(1.0), glm::vec3(0, -10, 0)));

	dynamic_cast<TransformNode*> (arm4)->setAnimationTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(0, 10, 0))
		*glm::rotate(glm::mat4(1.0), cos(time) / 2, glm::vec3(0.0f, 0.0f, 1.0f))
		*glm::translate(glm::mat4(1.0), glm::vec3(0, -10, 0)));

	dynamic_cast<TransformNode*> (sphere)->setAnimationTransform(glm::rotate(glm::mat4(1.0), time, glm::vec3(0.0f, 1.0f, 0.0f)));
	
	// Alec Animation 
	dynamic_cast<TransformNode*> (chasie)->setAnimationTransform(
		glm::translate(glm::mat4(1.0), glm::vec3(0, 200, 0))
		* glm::rotate(glm::mat4(1.0), sin(time*2), glm::vec3(1, 0, 0))
		* glm::translate(glm::mat4(1.0), glm::vec3(0, -200, 0))
		* glm::rotate(glm::mat4(1.0), 2 * time, glm::vec3(0, 1, 0))
		); 
}

void Scenegraph::createNodes()
{
	arm1 = root->getNode("arm1");
	arm2 = root->getNode("arm2");
	arm3 = root->getNode("arm3");
	arm4 = root->getNode("arm4");
	sphere = root->getNode("sphere-rotation");
	chasie = root->getNode("chasie");

	// for movement
	player = root->getNode("Player"); // need to add this object to the maze!!!
	checkPoint1 = root->getNode("CheckPoint1");
	checkPoint2 = root->getNode("CheckPoint2");
	checkPoint3 = root->getNode("CheckPoint3");
	checkPoint4 = root->getNode("CheckPoint4");
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