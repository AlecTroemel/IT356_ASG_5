#ifndef NODE_H
#define NODE_H

#define GLM_FORCE_RADIANS
#include <string>
#include <map>
#include <stack>
#include <utils/Light.h>
#include <iostream>
#include "Ray.h"
#include "Hitrecord.h"
using namespace std;
#include <glm/glm.hpp>

class Scenegraph;

class Node
{
protected:
    string name;
    Scenegraph *scenegraph;
	bool bbDraw;
public:
    Node(Scenegraph *graph,string name="")
    {
		this->parent = NULL;
        scenegraph = graph;
        setName(name);
    }

    virtual ~Node()
    {
	}

	virtual Node *getNode(string name)
	{
		if (this->name == name)
			return this;

		return NULL;
	}

    virtual void draw(stack<glm::mat4>& modelView)=0;
	virtual void getLights(stack<glm::mat4>& modelView, vector<graphics::Light *> *list) = 0;

	// calculate the intersection of ray with scenegraph, implimentations in leaf, transform, and group nodes
	virtual bool intersect(Ray R, Hitrecord & hr, stack<glm::mat4>& modelView) = 0; 

	virtual Node *clone()=0;
	void setParent(Node *parent)
	{
		this->parent = parent;
	}

	virtual void setScenegraph(Scenegraph *graph)
	{
		this->scenegraph = graph;
	}

    void setName(const string& name)
    {
        this->name = name;
    }

	void setBBDraw(bool d)
	{
		bbDraw = d;
	}

	void addLight(const graphics::Light& l)
	{
		lights.push_back(l);
		cout << "Light added in node " << name << endl;
	}


protected:
	Node *parent;
	vector<graphics::Light> lights;

};

#endif // NODE_H
