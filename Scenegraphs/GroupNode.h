#ifndef GROUPNODE_H
#define GROUPNODE_H

#include <GL/gl.h>
#include <vector>
using namespace std;
#include "Node.h"

class GroupNode: public Node
{
protected:
    vector<Node *> children;

public:
    GroupNode(Scenegraph *graph,string name="")
        :Node(graph,name)
    {

    }

    ~GroupNode()
    {
        for (unsigned int i=0;i<children.size();i++)
            delete children[i];
    }

	virtual Node *clone()
	{
		vector<Node *> newc;

		for (int i=0;i<children.size();i++)
		{
			newc.push_back(children[i]->clone());
		}

		GroupNode * newgroup = new GroupNode(scenegraph,name);

		for (int i=0;i<children.size();i++)
		{
			newgroup->addChild(newc[i]);
		}		
		return newgroup;

	}

	virtual Node* getNode(string name)
	{
		if (Node::getNode(name)!=NULL)
		{
			return Node::getNode(name);
		}

		int i=0;
		Node *answer = NULL;

		while ((i<children.size()) && (answer == NULL))
		{
			answer = children[i]->getNode(name);
			i++;
		}
		return answer;
	}

	void setScenegraph(Scenegraph * graph)
	{
		Node::setScenegraph(graph);
		for (int i=0;i<children.size();i++)
		{
			children[i]->setScenegraph(graph);
		}
	}

    virtual void draw(stack<glm::mat4> &modelView)
    {
        for (int i=0;i<children.size();i++)
        {
            children[i]->draw(modelView);
        }
    }

    void addChild(Node *child)
    {
        children.push_back(child);
		child->setParent(this);
    }

    const vector<Node *> getChildren() const
    {
        return children;
    }

	// for getting list of lights in view coordinate system
	virtual void getLights(stack<glm::mat4>& modelView, vector<graphics::Light*> *list)
	{
		// for each light in this node, transform it to the modelview and add it to the list 
		for (int i = 0; i < lights.size(); i++)
		{
			// copy the light with its position in the model view coordinates 
			graphics::Light * temp = new graphics::Light();
			temp->setAmbient(lights.at(i).getAmbient());
			temp->setDiffuse(lights.at(i).getDiffuse());
			temp->setSpecular(lights.at(i).getSpecular());

			glm::vec4 tempPos = modelView.top() * lights.at(i).getPosition(); // 
			temp->setPosition(glm::vec3(tempPos.x, tempPos.y, tempPos.z));

			glm::vec4 tempSpotDir = lights.at(i).getSpotDirection();

			temp->setSpotDirection(glm::vec3(tempSpotDir.x, tempSpotDir.y, tempSpotDir.z));
			temp->setSpotAngle(lights.at(i).getSpotAngle());

			// add the light to the list 
			list->push_back(temp);
		}


		for (int i = 0; i<children.size(); i++)
		{
			children[i]->getLights(modelView, list);
		}
	}


	//calculating intersection of the ray with the children of the node 
	virtual bool intersect(Ray R, Hitrecord & hr, stack<glm::mat4>& modelView)
	{
		bool hits = false;
		for (int i = 0; i<children.size(); i++)
		{
			bool hit = children[i]->intersect(R, hr, modelView);
			if (hit) hits = true; // record if we've hit anything
		}

		return hits;

	}
};

#endif // GROUP_H
