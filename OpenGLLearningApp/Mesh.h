#pragma once

#include <GL/glew.h>
class Mesh
{
private:
	GLuint VAO, VBO, IBO, indexCount;
public:
	void CreateMesh(GLint* indices, GLfloat* vertices, unsigned int numOfIndices,unsigned int numOfVertices);
	void RenderMesh();
	void ClearMesh();
	Mesh();
	~Mesh();
};

