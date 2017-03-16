#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include "ShaderManager.h"
#include <iostream>
#include <map>
#include "Model.h"

class Engine
{
private:
	
	//struct for transform
	struct transform
	{
		glm::vec3 location;
		glm::vec3 rotation;
		glm::vec3 size;
		glm::mat4 transMatrix;
	};

	struct rigidBody
	{
		glm::vec3 velocity;
		glm::vec3 force;
		float mass;
	};

	//object struct
	struct object
	{
		GLuint textureID;
		//to use everything in transform struct
		transform trans;
		//to use everything in the physics struct
		rigidBody physics;
		//enum to determine collider Type 
		enum colliders	{
							//no Colliders
							colliderless,
							//Axis aligned bounding box
							AABB,
							//Sphepre or circle bouding box
							sphere
						};
		colliders colliderType;
		Model models;
	};

	struct camera
	{
		//field of view
		float fOV;
		//aspect ratio
		float ratio;
		float near;
		float far;
		//transform
		transform trans;
		//rigidbody
		rigidBody rigidbody;
		//4x4 matrix
		glm::mat4 cameraMat;
	};
	camera cam;

	//variables
	GLFWwindow* GLFWwindowPtr;
	
	ShaderManager shaderManger;
	float currentTime;
	float previousTime;
	float deltaTime;
	
	//collection of objects
	object* objs = new object[3];
	
public:
	
	//methods
	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShaders();
	bool collidesWith(const object& one,const object& two);
	
	//constructors
	Engine();
	~Engine();
};

