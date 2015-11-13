#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <map>
#include <stack>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <utils/Object.h>
#include <utils/Texture.h>
#include "Node.h"

class Scenegraph
{    
	typedef struct
	{
		GLint positionLocation;
		GLint ambientLocation;
		GLint diffuseLocation;
		GLint specularLocation;
	} LightLocation;


    friend class LeafNode;
	friend class SceneXMLReader;
public:
    Scenegraph();
    ~Scenegraph();
    void makeScenegraph(Node *root);
    void initShaderProgram(GLint shaderProgram);
    void draw(stack<glm::mat4>& modelView);

	void addInstance(graphics::Object *in)
	{
		if (instances.count(in->getName())<1)
			instances[in->getName()] = in;
	}
	void addTexture(graphics::Texture *tex)
	{
		if (instances.count(tex->getName())<1)
			textures[tex->getName()] = tex;
	}


	void animate(float t);

	graphics::Object *getInstance(string name)
	{
		if (instances.count(name)<1)
			return NULL;

		return instances[name];
	}

	Node * getRoot()
	{
		return root;
	}
	void updateShader(GLuint program) {



	}




	// for player movement
	void movePlayer(float playerX, float playerY);
	void moveCheckpoint();
	int checkPointCounter;

private:
    Node *root;

	// for animation 
	Node *arm1;
	Node *arm2;
	Node *arm3;
	Node *arm4;
	Node *sphere;
	Node *chasie;

	// for player movement
	Node *player;
	Node *checkPoint1;
	Node *checkPoint2;
	Node *checkPoint3;
	Node *checkPoint4;

	void createNodes();
	vector<graphics::Light*> * lightsToViewCoord(stack<glm::mat4>& modelView);

	graphics::Texture deafultTexture;

	map<string,graphics::Object *> instances;
	map<string, graphics::Texture *> textures;
	GLint	projectionLocation,
			modelviewLocation,
			normalMatrixLocation, 
			numLightsLocation, 
			mat_ambientLocation, 
			mat_diffuseLocation,
			mat_specularLocation,
			mat_shininessLocation;

	GLint   texturematrixLocation,
			textureLocation;
	GLuint  textureID;

	LightLocation lightLocation[10];
	bool debug = true;
};

#endif // SCENEGRAPH_H
