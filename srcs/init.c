#include "morphosis.h"

t_fract						*init_fract(void)
{
	t_fract 				*fract;

	if (!(fract = (t_fract *)malloc(sizeof(t_fract))))
		error(MALLOC_FAIL_ERR, NULL);

	fract->p0.x = -1.5f;
	fract->p0.y = -1.5f;
	fract->p0.z = -1.5f;

	fract->p1.x = 1.5f;
	fract->p1.y = 1.5f;
	fract->p1.z = 1.5f;

	fract->grid_length = 3.0f;
	fract->grid_size = 0.0f;

	fract->grid.x = NULL;
	fract->grid.y = NULL;
	fract->grid.z = NULL;

	fract->step_size = 0.05f;

	fract->julia = init_julia();
	return fract;
}

t_julia 					*init_julia(void)
{
	t_julia 				*julia;

	if (!(julia = (t_julia *)malloc(sizeof(t_julia))))
		error(MALLOC_FAIL_ERR, NULL);

	julia->max_iter = 6;
	julia->threshold = 2.0f;
	julia->w = 0.0f;

	julia->c.x = -0.2f;
	julia->c.y = 0.8f;
	julia->c.z = 0.0f;
	julia->c.w = 0.0f;

	return julia;
}

t_data						*init_data(void)
{
	t_data 					*data;

	if (!(data = (t_data *)malloc(sizeof(t_data))))
		error(MALLOC_FAIL_ERR, NULL);
	data->gl = init_gl_struct();
	data->fract = init_fract();
	data->vertexpos = NULL;
	data->vertexval = NULL;
	data->triangles = NULL;
	return data;
}

void						init_vertex(t_data *data)
{
	size_t 					size;

	size = pow(data->fract->grid_size, 3) * 8;
	if (!(data->vertexpos = (float3 *)malloc(size * sizeof(float3))))
		error(MALLOC_FAIL_ERR, data);
	if (!(data->vertexval = (float *)malloc(size * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
}

void						init_grid(t_data *data)
{
	t_fract 				*f;

	f = data->fract;
	if (!(f->grid.x = (float *)malloc(((size_t)f->grid_size + 1) * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
	if (!(f->grid.y = (float *)malloc(((size_t)f->grid_size + 1) * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
	if (!(f->grid.z = (float *)malloc(((size_t)f->grid_size + 1) * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
}
