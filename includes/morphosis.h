#ifndef _MORPHOSIS_H
#define _MORPHOSIS_H

#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <errors.h>
#include <gl_includes.h>
#include <matrix.h>
#include <obj.h>

#define OUTPUT_FILE "./fractal.obj"
#define OUTPUT_PRECISION 3

t_data *init_data(void);
t_gl *init_gl_struct(void);
t_julia *init_julia(void);
t_fract *init_fract(void);
void init_grid(t_data *data);
void init_vertex(t_data *data);

void error(int errno, t_data *data);
float s_size_warning(float size);

float3 **arr_float3_cat(float3 **f_from, float3 **f_to, uint2 *len);
float3 **alloc_float3_arr(float3 **mem, uint2 *len);

void clean_up(t_data *data);
void clean_gl(t_gl *gl);
void clean_fract(t_fract *fract);
void clean_trigs(float3 **trigs, uint len);
void clean_calcs(t_data *data);

void calculate_point_cloud(t_data *data);
void calculate_point_cloud_optimized(t_data *data);
void create_grid(t_data *data);
void subdiv_grid(float start, float stop, float step, float *axis);
void define_voxel(t_fract *fract, float s);

void build_fractal(t_data *data);
void build_fractal_optimized(t_data *data);

float sample_4D_Julia(t_julia *julia, float3 pos);
float sample_4D_Julia_optimized(t_julia *julia, float3 pos);

float3 **polygonise(float3 *v_pos, float *v_val, uint2 *pos, t_data *data);

void export_obj(t_data *data);
void export_fractal_json(t_data *data, const char *filename);
void write_mesh(t_data *data, int surface, obj *o);

#endif
