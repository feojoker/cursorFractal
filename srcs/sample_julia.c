#include "morphosis.h"

float 						sample_4D_Julia(t_julia *julia, float3 pos)
{
	cl_quat 				z;
	uint 					iter;
	float					temp_mod;

	iter = 0;
	z.x = pos.x;
	z.y = pos.y;
	z.z = pos.z;
	z.w = julia->w;

	while (iter < julia->max_iter)
	{
		z = cl_quat_mult(z, z);
		z = cl_quat_sum(z, julia->c);
		temp_mod = cl_quat_mod(z);
		if (temp_mod > 2.0f)
			return 0.0f;
		iter++;
	}
	return 1.0f;
}
