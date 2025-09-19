#include "morphosis.h"

void 						run_graphics(t_gl *gl, float3 max, float3 min)
{
	gl_scale_tris(gl, max, min);

	init_gl(gl);
	createVAO(gl);
	createVBO(gl, gl->num_pts * sizeof(float), (GLfloat *)gl->tris);

	makeShaderProgram(gl);
	gl_set_attrib_ptr(gl, "pos", 3,3, 0);
	gl_calc_transforms(gl);
	gl_render(gl);

	terminate_gl(gl);
}

void							gl_render(t_gl *gl)
{
	float 					time;
	float					delta;
	float 					old_time;

	old_time = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(gl->window))
	{
		processInput(gl->window, gl);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		time = (float)glfwGetTime();
		delta = (time - old_time);
		glm_mat4_identity(gl->matrix->model_mat);
		glm_rotate(gl->matrix->model_mat, (0.25f * delta * glm_rad(180.0f)), gl->matrix->up);
		glUniformMatrix4fv(gl->matrix->model, 1, GL_FALSE, (float *)gl->matrix->model_mat);
		old_time = time;
		glm_rotate(gl->matrix->view_mat, (0.25f * delta * glm_rad(180.0f)), gl->matrix->up);
		glUniformMatrix4fv(gl->matrix->view, 1, GL_FALSE, (float *)gl->matrix->view_mat);

		glDrawArrays(GL_TRIANGLES, 0, gl->num_pts);

		glfwSwapBuffers(gl->window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
