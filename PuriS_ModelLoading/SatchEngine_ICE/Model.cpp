#include "Model.h"

void Model::render()
{
	//bind vertex
	glBindVertexArray(vertArr);
	//bind vertex
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}

bool Model::buffer(std::string filename)
{
	//vector collections
	std::vector<glm::vec3> locations = {};
	std::vector<glm::vec2> uvs = {};
	std::vector<glm::vec3> normals = {};
	std::vector<VertInd> vertInds = {};
	float flt;
	float flt1;
	float flt2;
	unsigned int num;
	unsigned int num1;
	unsigned int num2;
	char chr;
	char chr1;

	//load file
	std::string objFile = filename;
	//to open file
	std::ifstream infile;
	//variable to hold values from getline
	std::string line;
	infile.open(objFile);
	//loop to read file until empty
	while (std::getline(infile, line))
	{
		//string stream obj to sort the file
		std::istringstream ss(line);
		//to get first line
		std::string lineLabel;
		ss >> lineLabel;

		//if label is v
		if (lineLabel == "v")
		{
			//read 3 floats
			ss >> flt >> flt1 >> flt2;
			//add to vec3 locs
			locations.push_back({ glm::vec3(flt, flt1, flt2) });
		}
		//if label is vt
		else if (lineLabel == "vt")
		{
			//read 2 floats
			ss >> flt >> flt1;
			//add to uvs
			uvs.push_back({ glm::vec2(flt, flt1) });
		}
		//if label is vn
		else if (lineLabel == "vn")
		{
			//read 3 floats
			ss >> flt >> flt1 >> flt2;
			//add to normals
			normals.push_back({ glm::vec3(flt, flt1, flt2) });
		}
		//indices
		else if (lineLabel == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				//read unsigned int, then char
				//read unsigned int, then char
				//read unsigned int
				ss >> num >> chr >> num1 >> chr1 >> num2;
				//decrement each num by 1
				num -= 1;
				num1 -= 1;
				num2 -= 1;

				VertInd vert;
				vert.locInd = num;
				vert.uvInd = num1;
				vert.normInd = num2;
				//add to vertinds
				vertInds.push_back(vert);
			};
		}
	}
	//close the file if no lines left
	infile.close();
	vertCount = vertInds.size();
	//process model data
	//duplicate vert into a single buffer
	std::vector<vertex> vertBufData(vertCount);

	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = {
							locations[vertInds[i].locInd],
							uvs[vertInds[i].uvInd],
							normals[vertInds[i].normInd]
						};
	}
	/*
	//Define model vert location
	std::vector<glm::vec3> locs = {
		{ 1.0f, 1.0f, 0 },		//top right
		{ -1.0f, 1.0f, 0 },		//top left
		{ -1.0f, -1.0f, 0 },	//bot left
		{ 1.0f, -1.0f, 0 } };	//bot right

								//uvs
	std::vector<glm::vec2> uvs = {
		{ 1,1 },		//top right
		{ 0,1 },		//top left
		{ 0,0 },		//bot left
		{ 1,0 } };	//bot right

					//connect the dots
	std::vector <unsigned int>
		locInds = { 0, 1, 2,
		0, 2, 3 };
	*/
	

	//Setup GLuint = hold the num identifying the array and buffer generating

	//generating a buffer
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	//bind the buffer
	//glBindBuffer controls which array and buffer is active
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	//store data in the buff
	//glBufferData copies specified data into currently bound buffer
	glBufferData(GL_ARRAY_BUFFER,				//where to copy 
		sizeof(vertex) * vertCount,		// # bytes to copy
		&vertBufData[0],				//where to copy from
		GL_STATIC_DRAW);				//to OpenGL

										//describe the buffer layout
										//enable the attribute
	glEnableVertexAttribArray(0); //attribute index - 0

								  //setup the attribute
	glVertexAttribPointer(
		0,					//Attribute index - 0 in this case
		3,					//Number of componets (x, y, z)
		GL_FLOAT,			//Type of Data
		GL_FALSE,			//Should we normalize the data?
		sizeof(vertex),		//Stride ( Bytes per vert)
		0);					//Offset to this attribute


							//for UV
	glEnableVertexAttribArray(1); //attribute index - 1
								  //UV
	glVertexAttribPointer(
		1,					//Attribute index - 0 in this case
		2,					//Number of componets (x, y, z)
		GL_FLOAT,			//Type of Data
		GL_FALSE,			//Should we normalize the data?
		sizeof(vertex),		//Stride ( Bytes per vert)
		(void*)sizeof(glm::vec3));	//Offset to this attribute


	glBindVertexArray(0); //unbind when finished editing

						  //change window color
						  //only set once
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	
	//to enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	return true;
}

Model::Model()
{
	vertArr = 0;
	vertCount = 0;
	vertBuf = 0;
}


Model::~Model()
{
}
