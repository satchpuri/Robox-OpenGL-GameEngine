#pragma once
#include <iostream>
#include <FreeImage.h>
#include <GL/glew.h>
class Texture
{
private:
	//To explicitly define the height and width so window size doesnt change
	int height;
	int width;
	GLuint textureID;

public:
	//methods
	//method to load the textures
	GLuint LoadTextures(std::string fileName);

	Texture();
	~Texture();
};

