#include "Texture.h"

GLuint Texture::LoadTextures(std::string fileName)
{
	//getting texture data
	//to get the file type data
	FIBITMAP* filetype = FreeImage_Load(FreeImage_GetFileType(fileName.c_str(), 0), fileName.c_str());
	//if load fails
	if (filetype == nullptr)
	{
		std::cout << "Error: Texture Not Found!" << std::endl;
		return 0;
	}
	//to convert data into bitmap
	FIBITMAP* convertImg = FreeImage_ConvertTo32Bits(filetype);
	
	//width and height of images
	width = FreeImage_GetWidth(convertImg);
	height = FreeImage_GetHeight(convertImg);
	
	//Generate and bind texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(convertImg));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	//unload texture bytes
	FreeImage_Unload(filetype);
	glBindTexture(GL_TEXTURE_2D, 0);

	//validation
	std::cout << "Success: Texture Loaded!" << std::endl;
	//return GLuint
	return textureID;
}

Texture::Texture()
{
}


Texture::~Texture()
{
	//deletes texture
	glDeleteTextures(1, &textureID);
}
