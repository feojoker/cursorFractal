#include "morphosis.h"

void 						createVBO(t_gl *gl, GLsizeiptr size, GLfloat *points)
{
	glGenBuffers(1, &gl->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, gl->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_DYNAMIC_DRAW);
}

void						createVAO(t_gl *gl)
{
	glGenVertexArrays(1, &gl->vao);
	glBindVertexArray(gl->vao);
}
