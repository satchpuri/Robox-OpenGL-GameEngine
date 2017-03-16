#include "Engine.h"
#include "Texture.h"

namespace {
			//for key inputs
			std::map<int, bool> keyIsDown;
			std::map<int, bool> keyWasDown;
		}

#pragma region MouseClickCheck
//fuction to check clicks 
void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods)
{
	keyIsDown[button] = action;
}

//fuction for keypress
void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	keyIsDown[key] = action;
}
#pragma endregion

#pragma region initialize
bool Engine::init()
{
	//try catch to check if all steps succeed
	try
	{
		//Initialize the window lib
		//Glfwinit initializes glfw
		if (glfwInit() == GL_FALSE)
		{
			return false;
		}
		//create a windowed mode window
		GLFWwindowPtr = glfwCreateWindow(1920,1080, "SatchEngine DSA1", NULL, NULL);

		//set the click function
		glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
		//set the keypress fuction
		glfwSetKeyCallback(GLFWwindowPtr, keyCallback);
		//make it the currently active window, or quit.
		if (GLFWwindowPtr != nullptr)
		{
			glfwMakeContextCurrent(GLFWwindowPtr);
		}
		else
		{
			glfwTerminate();
			return false;
		}
		//Initialize GLEW or quit
		if (glewInit() != GLEW_OK)
		{
			glfwTerminate();
			return false;
		}
		return true;
	}
	//if all code doesnt succeed, return false. 
	catch(...)
	{
		return false;
	}
}
#pragma endregion

#pragma region buffer
bool Engine::bufferModels()
{
	if (!objs[0].models.buffer("models/models/quad.obj"))
	{
		return false;
	}
	if (!objs[1].models.buffer("models/models/sphere.obj"))
	{
		return false;
	}
	if (!objs[2].models.buffer("models/models/box.obj"))
	{
		return false;
	}
	else
	{
		return true;
	}
}
#pragma endregion


bool Engine::gameLoop()
{
	//Texture obj to load the textures
#pragma region Textures
	Texture texture;
	GLuint textureID = texture.LoadTextures("Textures/texture.bmp");
	GLuint textureID1 = texture.LoadTextures("Textures/Robot.bmp");
	GLuint textureID2 = texture.LoadTextures("Textures/box.bmp");
	//GLuint textureID3 = texture.LoadTextures("Textures/Texture4.bmp");

	//one.textureID = textureID;
	//two.textureID = textureID1;
	//three.textureID = textureID2;
	//one.trans.location = { 0, 0, 0 };
	//two.trans.location = { -.5,-.5,-1 };
	//three.trans.location = { .8,-.6,-1 };
	//one.trans.rotation = { 0,0,0 };
	//two.trans.rotation = { 0,0,0 };
	//three.trans.rotation = { 0,0,0 };
	//one.trans.size = { 1, 1,1 };
	//two.trans.size = { -0.1, 0.3, 0.1 };
	//three.trans.size = {0.08, 0.15, 0.08 };

	//initializing obj values
	//assinging textureID
	objs[0].textureID = textureID;
	objs[1].textureID = textureID1;
	objs[2].textureID = textureID2;
#pragma endregion

#pragma region Transformations
	//assigning location
	objs[0].trans.location = { 0, 0, -.3 };
	objs[1].trans.location = { -.5,-.5,0 };
	objs[2].trans.location = { .8,-.6,0 };
	//assigning rotation
	objs[0].trans.rotation = { 0, 0, 0 };
	objs[1].trans.rotation = { 0, 0, 0 };
	objs[2].trans.rotation = { 0,0,0 };
	//assigning scale
	objs[0].trans.size = { -1.5, 1, 1 };
	objs[1].trans.size = { 0.07, 0.3, 0.07 };
	objs[2].trans.size = { 0.08, 0.15, 0.08 };

#pragma endregion
	
#pragma region Phsyics
	//assigning force
	objs[0].physics.force = { .5,0,0 };
	objs[1].physics.force = { .6,0,0 };
	objs[2].physics.force = { 0,.5,0 };
	//asssigning mass
	objs[0].physics.mass = 1;
	objs[1].physics.mass = 1;
	objs[2].physics.mass = 3;
	//assigning velocity
	objs[0].physics.velocity = { 0,0,0 };
	objs[1].physics.velocity = { 0,0,0 };
	objs[2].physics.velocity = { 0,0,0 };
#pragma endregion

#pragma region Colliders
	//assigning collider type
	objs[0].colliderType = objs[0].colliderless;
	objs[1].colliderType = objs[1].AABB;
	objs[2].colliderType = objs[2].AABB;
#pragma endregion
	
	//to give the location in the GLSL fragment sharder file
	GLuint location = glGetUniformLocation(shaderManger.getProgram(), "myTexture");
	//pass in the variable
	glUniform1i(location, 0);

	currentTime = glfwGetTime();
	previousTime = glfwGetTime();

#pragma region Camera
	//camera values
	cam.trans.location = { 0,0,1 };

	cam.rigidbody.velocity = { 0,0,0 };
	cam.rigidbody.force = {.5,.5,.5 };
	cam.rigidbody.mass = 3;

	cam.fOV = 3.14159f * .4f / 1.f;
	cam.far = 1000.f;
	cam.near = .01f;
	// 1920/1080
	cam.ratio = 16.f / 9.f;
#pragma endregion

#pragma region GameLoop
	//GAME LOOP
	//Loop until the user closes the window
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
#pragma region Lookat and Perspective Matrix
		//camera stuff
		//rotion mat
		glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(cam.trans.rotation.y, cam.trans.rotation.x, cam.trans.rotation.z);
		//Look at view matrix
		//lookat = lookat(eye, center, up)
		glm::mat4 lookAtMat = glm::lookAt(cam.trans.location, cam.trans.location + rotMat * glm::vec3(0, 0, -1), rotMat * glm::vec3(0, 1, 0));
		
		//prespective view
		//prespective = perspective(fov, ratio, near, far);
		glm::mat4 perspective = glm::perspective(cam.fOV, cam.ratio, cam.near, cam.far);
		//camera mat
		cam.cameraMat = perspective * lookAtMat;
#pragma endregion

		//to set previous time as the previous frame
		previousTime = currentTime;
		//to update current time
		currentTime = glfwGetTime();
		//deltatime
		deltaTime = currentTime - previousTime;

		//clear the canvas
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
#pragma region CollisionDetection
		//check for collision
		if (collidesWith(objs[1], objs[2]))
		{
			//if colliding move back the robot
			objs[1].trans.location.x = objs[2].trans.location.x - 0.15;
			std::cout << "Collision Detected" << endl;
			std::cout << "Collision Type: " << objs[1].colliderType << "and " << objs[2].colliderType << endl;
		}
#pragma endregion

#pragma region DrawModels
		//Draw buffered models
		//uploading the matrix
		glUniformMatrix4fv(3, 1, GL_FALSE, &cam.cameraMat[0][0]);
		//rendering game objs
		for (int i = 0; i < 3; i++)
		{
			//world matrix
			objs[i].trans.transMatrix = (glm::translate(objs[i].trans.location)) * (glm::yawPitchRoll(objs[i].trans.rotation.y, objs[i].trans.rotation.x, objs[i].trans.rotation.z)) * (glm::scale(objs[i].trans.size));
			//world transform
			glUniformMatrix4fv(2, 1, GL_FALSE, &objs[i].trans.transMatrix[0][0]);
			//bind texture
			glBindTexture(GL_TEXTURE_2D, objs[i].textureID);

			//to render the obj
			objs[i].models.render();
		
		}
#pragma endregion

#pragma region MoveObject

		//Process input/window events
		//key Inputs 
		if (keyIsDown[GLFW_KEY_F])
		{
			//fps
			std::cout << "Delta Time:" << 1.0f / deltaTime << std::endl;
		}
		if (keyIsDown[GLFW_KEY_D])
		{
			objs[1].physics.velocity.x = 1;
		}
		if (keyIsDown[GLFW_KEY_A])
		{
			objs[1].physics.velocity.x = -1;
		}
		if (keyIsDown[GLFW_KEY_SPACE])
		{	
			objs[1].physics.velocity.y = 2;
		}
		if (!keyIsDown[GLFW_KEY_D] && (!keyIsDown[GLFW_KEY_A]))
		{
			objs[1].physics.velocity.x = 0;
			
		}
		if (!keyIsDown[GLFW_KEY_SPACE])
		{
			objs[1].physics.velocity.y = 0;
		}
#pragma endregion

#pragma region UpdatePhysics
		//Update physical sims
		if (objs[1].trans.location.y >= -0.5)
		{
			objs[1].physics.force.y += -500;
		}
		if (objs[1].trans.location.y < -0.5)
		{
			objs[1].trans.location.y = -.5;
		}

		objs[1].physics.velocity += (objs[1].physics.force / objs[1].physics.mass) * deltaTime;
		objs[1].trans.location += objs[1].physics.velocity * deltaTime;
		objs[1].physics.force = glm::vec3(0, 0, 0);
#pragma endregion

#pragma region MoveCamera
		//to move the camera
		if (keyIsDown[GLFW_KEY_RIGHT])
		{
			cam.rigidbody.velocity.x = 1;
		}
		if (keyIsDown[GLFW_KEY_LEFT])
		{
			cam.rigidbody.velocity.x = -1;
		}
		if (keyIsDown[GLFW_KEY_UP])
		{
			cam.rigidbody.velocity.y = 1;
		}
		if (keyIsDown[GLFW_KEY_DOWN])
		{
			cam.rigidbody.velocity.y = -1;
		}
		if (!keyIsDown[GLFW_KEY_LEFT] && (!keyIsDown[GLFW_KEY_RIGHT]))
		{
			cam.rigidbody.velocity.x = 0;
		}
		if (!keyIsDown[GLFW_KEY_UP] && (!keyIsDown[GLFW_KEY_DOWN]))
		{
			cam.rigidbody.velocity.y = 0;
		}
#pragma endregion

#pragma region CameraPhysics

		cam.rigidbody.velocity += (cam.rigidbody.force / cam.rigidbody.mass) * deltaTime;
		cam.trans.location += cam.rigidbody.velocity * deltaTime;
		cam.rigidbody.force = glm::vec3(0, 0, 0);

#pragma endregion

		//swap the front (what the screen displays) and the back (what openGL draws) buffers
		glfwSwapBuffers(GLFWwindowPtr);
		//to close the window
		
		if (keyIsDown[GLFW_KEY_ESCAPE])
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		}
		
		keyWasDown = keyIsDown;
		glfwPollEvents(); 
	}
#pragma endregion

	//terminate glfw to avoid mem loss
	glfwTerminate();

	return true;
}

bool Engine::useShaders()
{
	if (shaderManger.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl") == true)
	{
		glUseProgram(shaderManger.getProgram());
		return true;
	}
	return false;
}

//return false if the objs arent colliding 
bool Engine::collidesWith(const object& one,const object& two)
{
	//if no collider = not colliding 
	//radius = size.x/2 
	//location = |two - one|
	if (one.colliderType == one.colliderless || two.colliderType == two.colliderless)
	{
		return false;
	}
	//sphere
	if (one.colliderType == one.sphere && two.colliderType == two.sphere)
	{
		//if location > one.size.x + two.size.x = separated
		if (abs(two.trans.location.x - one.trans.location.x) > ((one.trans.size.x/2) + (two.trans.size.x/2)))
		{
			//separated
			return false;
		}
		//collding
		return true;
	}

	//AABB
	if (one.colliderType == one.AABB && two.colliderType == two.AABB)
	{
		//If sepation on x > one.size.x + two.size.x = false
		//x-axis
		//one.size.x * -1 
		if (abs(two.trans.location.x - one.trans.location.x) > ((one.trans.size.x) + (two.trans.size.x)))
		{
			//separated
			return false;
		}
		//y-axis
		if (abs(two.trans.location.y - one.trans.location.y) > ((one.trans.size.y) + (two.trans.size.y)))
		{
			//separated
			return false;
		}
		//z-axis
		if (abs(two.trans.location.z - one.trans.location.z) > ((one.trans.size.z) + (two.trans.size.z)))
		{
			//separated
			return false;
		}
		//collding
		return true;
	}
	//one = AABB and two = Sphere
	if (one.colliderType == one.AABB && two.colliderType == two.sphere) 
	{
		//right side
		if ((two.trans.location.x - one.trans.location.x) > 0)
		{
			//if |size - location| > size = separated
			if (abs((two.trans.size.x / 2) - one.trans.location.x) > (two.trans.size.x / 2))
			{
				//seperated
				return false;
			}	
		}
		//left side
		if (two.trans.location.x - one.trans.location.x < 0)
		{
			//location.x - size.x = minx
			if (abs((one.trans.location.x - one.trans.size.x) - (two.trans.size.x / 2) > 0))
			{
				//sepatared 
				return false;
			}
		}
		//colliding
		return true;
	}
	//one = sphere and two = AABB
	if (one.colliderType == one.sphere && two.colliderType == two.AABB)
	{
		//right side
		if ((one.trans.location.x - two.trans.location.x) > 0)
		{
			//if |size - location| > size = separated
			if (abs((one.trans.size.x / 2) - two.trans.location.x) > (one.trans.size.x / 2))
			{
				//seperated
				return false;
			}
		}
		//left side
		if (one.trans.location.x - two.trans.location.x < 0)
		{
			//location.x - size.x = minx
			if (abs((two.trans.location.x - one.trans.size.x) - (one.trans.size.x / 2) > 0))
			{
				//sepatared 
				return false;
			}
		}
		//colliding
		return true;
	}
	return false;
}





Engine::Engine()
{
	
}

Engine::~Engine()
{
	//deleting textures in texures destructor
	//delete objs;
}
