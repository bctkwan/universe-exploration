#include "Bind_Data.h"
#include "Normal_mapping.h"
#include "Loading_func.h"

GLuint earthVao;
GLuint helicopterVao;
GLuint skyboxVao;
GLuint rockVao;
GLuint starVao;
int drawEarthSize = 0;
int drawHelicopterSize = 0;
int drawRockSize = 0;
int drawStarSize = 0;

extern unsigned int rockAmount;
extern unsigned int maxStarAmount;
std::vector<glm::vec3> earthVertices;
std::vector<glm::vec3> helicopterVertices;
GLuint star_instance_buffer;

void bindEarth(const char * path)
{
	//Earth data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n, earthVertices);

	/*
	* Source code of "Tutorial 13 : Normal Mapping" on "opengl-tutorial.org"
	* http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
	* Under the WTFPL Public License
	* Source code obtained from:
	* https://github.com/opengl-tutorials/ogl/blob/master/tutorial13_normal_mapping/tutorial13.cpp
	*/
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	computeTangentBasis(
		vao5_v, vao5_uvs, vao5_n, // input
		tangents, bitangents    // output
	);
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
	indexVBO_TBN(
		vao5_v, vao5_uvs, vao5_n, tangents, bitangents,
		indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
	);

	//sending earth data
	glGenVertexArrays(1, &earthVao);
	glBindVertexArray(earthVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//tangent data
	GLuint tangent_buffer;
	glGenBuffers(1, &tangent_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//bitangent data
	GLuint bitangent_buffer;
	glGenBuffers(1, &bitangent_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//index data
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	//draw data
	drawEarthSize = indices.size();
}

void bindHelicopter(const char * path)
{
	//Helicopter data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n, helicopterVertices);

	//sending helicopter data
	glGenVertexArrays(1, &helicopterVao);
	glBindVertexArray(helicopterVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawHelicopterSize = vao5_v.size();
}

void bindSkybox()
{
	// Cubemap
	GLfloat skyboxVertices[] =
	{
		// Positions
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVao);
	glBindVertexArray(skyboxVao);
	GLuint skyboxVbo;
	glGenBuffers(1, &skyboxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void bindRock(const char * path)
{
	//Rock data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;
	std::vector<glm::vec3> temp_vertices;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n, temp_vertices);

	//sending rock data
	glGenVertexArrays(1, &rockVao);
	glBindVertexArray(rockVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawRockSize = vao5_v.size();
	
	// instanced rendering
	glm::mat4 *instanceMatrices;
	instanceMatrices = new glm::mat4[rockAmount];
	float radius = 6.0;
	float offset = 2.5f;
	for (unsigned int i = 0; i < rockAmount; i++)
	{
		glm::mat4 instanceMatrix;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float) i / (float) rockAmount * 360.0f;
		float displacement = (rand() % (int) (2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int) (2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int) (2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		instanceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

		// 2. scale: Scale between 0.01 and 0.05f
		float scale = (rand() % 20) / 500.0f + 0.01;
		instanceMatrix = glm::scale(instanceMatrix, glm::vec3(scale));
		
		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		instanceMatrix = glm::rotate(instanceMatrix, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		
		// 4. now add to list of matrices
		instanceMatrices[i] = instanceMatrix;
	}

	// vertex Buffer Object
	GLuint instance_buffer;
	glGenBuffers(1, &instance_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, rockAmount * sizeof(glm::mat4), &instanceMatrices[0], GL_STATIC_DRAW);
	// vertex Attributes
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*) 0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*) (vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*) (2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*) (3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
	
}

void bindStar(const char * path)
{
	//Star data
	std::vector<glm::vec3> vao5_v;
	std::vector<glm::vec2> vao5_uvs;
	std::vector<glm::vec3> vao5_n;
	std::vector<glm::vec3> temp_vertices;

	loadOBJ(path, vao5_v, vao5_uvs, vao5_n, temp_vertices);

	//sending star data
	glGenVertexArrays(1, &starVao);
	glBindVertexArray(starVao);
	GLuint vbo5ID;
	glGenBuffers(1, &vbo5ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5ID);
	glBufferData(GL_ARRAY_BUFFER, vao5_v.size() * sizeof(glm::vec3), &vao5_v[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//UV data
	GLuint vbo5uvbuffer;
	glGenBuffers(1, &vbo5uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_uvs.size() * sizeof(glm::vec2), &vao5_uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normal data
	GLuint vbo5nor_buffer;
	glGenBuffers(1, &vbo5nor_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo5nor_buffer);
	glBufferData(GL_ARRAY_BUFFER, vao5_n.size() * sizeof(glm::vec3), &vao5_n[0], GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//draw data
	drawStarSize = vao5_v.size();

	// instanced rendering
	// vertex Buffer Object
	glGenBuffers(1, &star_instance_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, star_instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, maxStarAmount * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	// vertex Attributes
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

}
