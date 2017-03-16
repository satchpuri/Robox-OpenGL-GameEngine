#include "ShaderManager.h"

//getter
GLuint ShaderManager::getProgram() const
{
	return program;
}

GLuint ShaderManager::loadShader(const char* file, GLenum shaderType)
{
	//Load the file
	std::ifstream inFile(file, std::ios::binary);
	//char pointer array to hold the file
	char* fileContents; 

	//if file can't be read
	if (!inFile.is_open())
	{
		cout << "Error: File cannot be read!" << std::endl;
		return 0;
	}
	//to read file
	else
	{
		//seek to the end to get the length 
		inFile.seekg(0, std::ios::end);
		int length = (int)inFile.tellg();
		//get back to the start
		inFile.seekg(0, std::ios::beg);
		//array to hold the file
		//DELETE array (reminder)
		fileContents = new char[length + 1];
		//Read the file
		inFile.read(fileContents, length);
		//set null pointer
		fileContents[length] = '\0';
		//close the file
		inFile.close();
	}

	//create shader
	GLuint shader = glCreateShader(shaderType);
	//create shader source
	glShaderSource(shader, 1, &fileContents, 0);
	//compile
	glCompileShader(shader);
	//delete the array
	delete[] fileContents;

	//if it loads correctly
	//hold the info log length
	GLint result; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE)
	{
	cout << "Success: Shader loaded!" << endl;

		return shader;
	}
	//if it didnt complile
	else
	{
		cout << "Error: Shader couldn't load!" << endl;

		//to access the log
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		//array for the log
		//DELETE array (reminder)
		GLchar* log = new GLchar[logLength];
		//to get shader log info
		glGetShaderInfoLog(shader, logLength, 0, log);
		//print out log report
		cout << log;
		//delete array
		glDeleteShader(shader);
		delete[] log;

		return 0;
		}
}

bool ShaderManager::loadShaders(const char* vertexFile, const char* fragmentFile)
{
	GLuint vertexShader;
	GLuint fragmentShader;

	// to see if shaders loaded 
	if(vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER) )
	{
		//if loaded
		//vertexShader = loadShader(vertexFile, GL_VERTEX_SHADER);
		cout << "Success: Shader loaded!" << endl;
	}
	//if didnt
	else
	{
		cout << "Error: Shader couldn't load!" << endl;
		return 0;
	}

	//to see if fragment shaders loaded
	if(fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER))
	{
		//fragmentShader = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
		cout << "Success: Shader loaded!" << endl;
	}
	//if didnt
	else
	{
		cout << "Error: Shader couldn't load!" << endl;
		return 0;
	}

	//create shader program
	program = glCreateProgram();
	//include the shaders
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	//link program
	glLinkProgram(program);

	// to check the program link
	
	//if linked
	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_TRUE)
	{
		cout << "Success: Program linked!" << endl;
		return true;
	}
	
	//if program didnt link
	else
	{
		cout << "Error: Program linking error!" << endl;
		// Get the log report
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		//array for the log
		//DELETE array (reminder)
		GLchar* log = new GLchar[logLength];
		//get log info
		glGetProgramInfoLog(program, logLength, 0, log);

		//print log report
		cout << log;

		// Deletes
		glDeleteProgram(program);
		delete[] log;

		return false;
	}
}

ShaderManager::ShaderManager()
{
	program = 0;
}

ShaderManager::~ShaderManager()
{
}
