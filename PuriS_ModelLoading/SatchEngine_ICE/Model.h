#pragma once
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <iostream>
#include <map>
#include <sstream>
#include "ShaderManager.h"
#include "Texture.h"
#include <map>
#include <sstream>


struct vertex 
{
	glm::vec3 loc;
	glm::vec2 uv;
	glm::vec3 norm;
};

struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;

};
class Model
{
private:
	unsigned int vertCount;
	GLuint vertArr;
	GLuint vertBuf;
	
public:
	void render();
	bool buffer(std::string filename);
	Model();
	~Model();
};

