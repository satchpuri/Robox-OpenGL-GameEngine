#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include "Engine.h"

int main()
{
	//Engine obj
	Engine engine;

	//if init() returns false then return -1
	if (engine.init() == false)
	{
		return -1;
	}
	//if buffer fails return -1
	if (engine.bufferModels() == false)
	{
		return -1;
	}

	//call game loop if shadermanager is true
	if (engine.useShaders() == true)
	{
		//game loop 
		engine.gameLoop();
	}
	else
	{
		return -1;
	}

	return 0;
}