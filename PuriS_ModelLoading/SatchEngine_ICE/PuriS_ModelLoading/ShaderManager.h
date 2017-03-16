#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
using std::cout;
using std::endl;


class ShaderManager
{
private: 
	//index where the loaded shaders are stored on the graphics card
	GLuint program;
	//helper method to read and complie a shader file and return the index wher it is stored
	GLuint loadShader(const char* file, GLenum shaderType);
public:
	//methods
	//Getter read-only const
	GLuint getProgram() const;
	//method to load shaders. 
	bool loadShaders(const char* vertexFile, const char* fragmentFile);
	
	//constructor
	ShaderManager();
	~ShaderManager();
};

