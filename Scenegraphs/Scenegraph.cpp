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
float* Scenegraph::Raytrace(const int width, const int height, stack<glm::mat4>& modelView)
{
	const int size = width * height * 4;
	float *pixels = new float[size];

	// do the casting stuuuufffffff
	int counter = 0;
	for (int y = height-1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			// create Ray from camera through point (x,y)
			Ray R(x - (0.5 * width), y - (0.5 * height), -(0.5 * height) / tan(100.0f*3.14159f / 90));

			glm::vec4 color;
			Raycast(R, modelView, color);

			pixels[counter] = color.x;
			counter++;
			pixels[counter] = color.y;	
			counter++;
			pixels[counter] = color.z;
			counter++;
			pixels[counter] = 1;
			counter++;
		}
	}

	// return  the image as a 1d array of vec3
	return pixels;
}

bool Scenegraph::Raycast(Ray R, stack<glm::mat4>& modelView, glm::vec4 &color)
{
	Hitrecord hr;

	bool hit = root->intersect(R, hr, modelView);

	if (hit == true) 
	{
		//color = glm::vec4(1, 1, 1, 1);
		color = Shade(hr, modelView);
	}
	else // did not hit anything, so return background color
	{
		color = glm::vec4(1, 1, 1, 1);
	}

	return hit;
}

glm::vec4 Scenegraph::Shade(Hitrecord & hr, stack<glm::mat4>& modelview)
{
	glm::vec3 lightVec, viewVec, reflectVec;
	glm::vec3 normalView;
 	glm::vec3 ambient, diffuse, specular;
 	float nDotL, rDotV;

	glm::vec4 fColor = glm::vec4(0, 0, 0, 1);;

	const int MAXLIGHTS = 10;

	vector<graphics::Light*> * lights = lightsToViewCoord(modelview);
	int numLights = lights->size();

	viewVec = -glm::vec3(hr.getHitPoint());
	viewVec = glm::normalize(viewVec);

	for (int i = 0; i<numLights; i++)
	{	
		if (lights->at(i)->getPosition().w != 0)
		{
			lightVec = glm::normalize(glm::vec3(lights->at(i)->getPosition() - hr.getHitPoint()));
		}
		else
		{
			lightVec = glm::normalize(glm::vec3(lights->at(i)->getPosition()));
		}

		glm::vec3 normalView = glm::vec3(hr.getNormal());
		nDotL = glm::dot(normalView, lightVec);
		
		reflectVec = glm::reflect(-lightVec, normalView);
		reflectVec = glm::normalize(reflectVec);

		rDotV = max(glm::dot(reflectVec, viewVec), 0.0f);
		
		ambient = glm::vec3(hr.getMaterial().getAmbient()) * lights->at(i)->getAmbient();
		diffuse = glm::vec3(hr.getMaterial().getDiffuse()) * lights->at(i)->getDiffuse() * max(nDotL, 0.0f);	
		if (nDotL>0)
			specular = glm::vec3(hr.getMaterial().getSpecular()) * lights->at(i)->getSpecular() * pow(rDotV*-1, hr.getMaterial().getShininess());
		else
			specular = glm::vec3(0, 0, 0);
		
		fColor = fColor + glm::vec4(ambient + diffuse + specular, 1.0);

		fColor.x = min(fColor.x, 1.0f);
		fColor.y = min(fColor.y, 1.0f);
		fColor.z = min(fColor.z, 1.0f);
	}

	// add texture color 
	float textureR, textureG, textureB;
	hr.getTexture()->lookup(hr.getTextureCoord().x, hr.getTextureCoord().y, textureR, textureG, textureB);
	fColor = fColor * glm::vec4(textureR, textureG, textureB, 0);
	return fColor;
}










	/*
	vector<graphics::Light*> * lights = lightsToViewCoord(modelView);


	glm::vec3 lightVec, viewVec, reflectVec;
	glm::vec3 normalView;
	glm::vec3 ambient, diffuse, specular;
	float nDotL, rDotV;

	glm::vec3 fNormal;
	glm::vec4 fPosition;
	glm::vec4 fTexCoord;

	glm::vec4 fColor = glm::vec4(0, 0, 0, 1);

	glm::vec3 fPposition;
	


	for (int i = 0; i < lights->size(); i++)
	{
		glm::vec3 lightPosXYZ = glm::vec3(lights->at(i)->getPosition().x,
			lights->at(i)->getPosition().y,
			lights->at(i)->getPosition().z);

		if (lights->at(i)->getPosition().w != 0)
			lightVec = glm::normalize(lightPosXYZ - fPosition.xyz);
		else
			lightVec = glm::normalize(-lightPosXYZ);

		glm::vec3 tNormal = fNormal;
		normalView = glm::normalize(hr.normal);
		nDotL = glm::dot(normalView, lightVec);

		viewVec = -fPosition.xyz;
		viewVec = glm::normalize(viewVec);

		reflectVec = glm::reflect(-lightVec, normalView);
		reflectVec = glm::normalize(reflectVec);

		rDotV = max(glm::dot(reflectVec, viewVec), 0.0f);

		glm::vec3 materialAmbience = glm::vec3(hr.material.getAmbient().x, hr.material.getAmbient().y, hr.material.getAmbient().z);
		glm::vec3 materialDiffuse = glm::vec3(hr.material.getDiffuse().x, hr.material.getDiffuse().y, hr.material.getDiffuse().z);
		glm::vec3 materialSpecular = glm::vec3(hr.material.getSpecular().x, hr.material.getSpecular().y, hr.material.getSpecular().z);


		ambient = materialAmbience * lights->at(i)->getAmbient();
		diffuse = materialDiffuse * lights->at(i)->getDiffuse() * max(nDotL, 0.0f);
		if (nDotL>0)
			specular = materialSpecular * lights->at(i)->getSpecular() * pow(rDotV, hr.material.getShininess());
		else
			specular = glm::vec3(0, 0, 0);
		fColor = fColor + glm::vec4(ambient + diffuse + specular, 1.0);
	}
	return fColor; */