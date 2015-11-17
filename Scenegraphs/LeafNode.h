#ifndef _LEAF_H_
#define _LEAF_H_
#include "node.h"
#include <sstream>

#include <utils/Object.h>
#include <utils/Material.h>
#include "Intersections.h"
class LeafNode : public Node
{
public:

	LeafNode(graphics::Object *instanceOf,Scenegraph *graph,string name="")
		:Node(graph,name)
	{
		this->instanceOf = instanceOf;
		//default material
		material.setAmbient(1.0f,0.6f,0.6f);
		material.setDiffuse(1.0f,0.6f,0.6f);
		material.setSpecular(0.2f,0.1f,0.1f);
		material.setShininess(1);
	}

	~LeafNode(void)
	{
	}

	Node *clone()
	{
		LeafNode *newclone = new LeafNode(instanceOf,scenegraph,name);
		newclone->setMaterial(material);

		return newclone;
	}

	virtual void draw(stack<glm::mat4> &modelView)
    {
		GLuint a;
        if (instanceOf!=NULL)
		{

            //set the color for all vertices to be drawn for this object
            //glUniform4fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			//glVertexAttrib3fv(scenegraph->objectColorLocation, glm::value_ptr(color));

			glUniformMatrix4fv(scenegraph->modelviewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
			
			// send normal matrix to shader
			glUniformMatrix4fv(scenegraph->normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelView.top()))));

			// send material to shader
			glUniform3fv(scenegraph->mat_ambientLocation, 1, glm::value_ptr(this->material.getAmbient()));
			glUniform3fv(scenegraph->mat_diffuseLocation, 1, glm::value_ptr(this->material.getDiffuse()));
			glUniform3fv(scenegraph->mat_specularLocation, 1, glm::value_ptr(this->material.getSpecular()));
			glUniform1f(scenegraph->mat_shininessLocation, this->material.getShininess());

			a = glGetError();

			// Texture part
			glEnable(GL_TEXTURE_2D);	// enable texture mapping, do not ignore the texture mapping commands


			// use the default white texture if the node does not have one
			if (textures.empty()) 
			{
				glActiveTexture(GL_TEXTURE0); // an object can have at least 8 textures 
				glBindTexture(GL_TEXTURE_2D, scenegraph->deafultTexture.getTextureID());
			}
			else
			{
				for (int i = 0; i < textures.size(); i++) {
					stringstream name;
					name << "GL_TEXTURE" << i;
					GLint nameInt;
					name >> nameInt;
					glActiveTexture(nameInt); // an object can have at least 8 textures 
					glBindTexture(GL_TEXTURE_2D, textures[i].getTextureID());
				}
			}

			// bind what is currently bound to texture 
			glUniform1i(scenegraph->textureLocation, 0); //bind GL_TEXTURE0 to sampler2D (whatever is bound to GL_TEXTURE0)

			// you scan scale and modify the image to make it repeat/ rotate

			if (textures.size() > 0)
			{
				if (textures.at(0).getName() == "grass")
				{
					instanceOf->setTextureTransform(glm::scale(glm::mat4(1.0), glm::vec3(20, 20, 20)));
				}
				else
				{
					instanceOf->setTextureTransform(glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 1)));
				}
			}

			glUniformMatrix4fv(scenegraph->texturematrixLocation, 1, GL_FALSE, glm::value_ptr(instanceOf->getTextureTransform()));
			
			// send modelView to Matrix
			a = glGetError();
			instanceOf->draw(GL_TRIANGLES);        
			a = glGetError();
		}
    }

	virtual void getLights(stack<glm::mat4>& modelView, vector<graphics::Light *> *list)
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
			//cout << this->name << tempPos.x << " " << tempPos.y << " " << tempPos.z << endl;
			temp->setPosition(glm::vec3(tempPos.x, tempPos.y, tempPos.z));

			glm::vec4 tempSpotDir = lights.at(i).getSpotDirection();
			temp->setSpotDirection(glm::vec3(tempSpotDir.x, tempSpotDir.y, tempSpotDir.z));
			temp->setSpotAngle(lights.at(i).getSpotAngle());

			// add the light to the list 
			list->push_back(temp);
		}
	}

	

	glm::vec4 getColor()
	{
		return material.getAmbient();
	}

	/*
	 * Set the material of each vertex in this object
	 */
	virtual void setMaterial(graphics::Material& mat)
	{
		//cout << "material of node " << this->name << " updated" << endl;

		material = mat;
	}

	/*
	 * gets the material
	 */
	graphics::Material getMaterial()
	{
		return material;
	}

	void setTexture(graphics::Texture *tex)
	{
		textures.push_back(*tex);
		cout << "Texture set to " << tex->getName() << endl;
	}

	//calculating intersection of the ray with the child of the node after transformation
	virtual bool intersect(Ray R, Hitrecord & hr, stack<glm::mat4>& modelView)
	{
		Intersection intersection;
		string objectType = instanceOf->getName();
		//cout << objectType << endl;
		float newT = 0;
		bool hit = false;
		glm::vec3 normal;

		// convert Ray object coordinates 
		Ray rayObjectView = R;
		rayObjectView.start = glm::inverse(modelView.top()) * R.start;
		rayObjectView.dir = glm::inverse(modelView.top()) * R.dir;

		if (objectType == "box") hit = intersection.Box(newT, rayObjectView);
		else if (objectType == "sphere") hit = intersection.Sphere(newT, rayObjectView);

	
		// if the new T is closer, update the hitrecord 
		if (newT < hr.t)
		{
			hr.t = newT;
			hr.material = this->material;

			// need to update normal
			if (objectType == "box")
			{
				// hr.normal = stuff
			}
			else if (objectType == "sphere")
			{
				glm::vec4 P0 = rayObjectView.start + newT * rayObjectView.dir;
				// my stuff here
				hr.normal = (glm::transpose(glm::inverse(modelView.top())) * P0).xyz();
			}

			// later update textures
		}

		return hit;
	}

protected:
	graphics::Object *instanceOf;
	graphics::Material material;
	vector<graphics::Texture> textures;

};
#endif
