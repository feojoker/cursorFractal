#include "look-up.h"
#include "morphosis.h"

static uint getCubeIndex(float *v_val, uint pos) {
  uint cubeindex;

  cubeindex = 0;
  if (v_val[pos + 0])
    cubeindex |= 1;
  if (v_val[pos + 1])
    cubeindex |= 2;
  if (v_val[pos + 2])
    cubeindex |= 4;
  if (v_val[pos + 3])
    cubeindex |= 8;
  if (v_val[pos + 4])
    cubeindex |= 16;
  if (v_val[pos + 5])
    cubeindex |= 32;
  if (v_val[pos + 6])
    cubeindex |= 64;
  if (v_val[pos + 7])
    cubeindex |= 128;
  return cubeindex;
}

static float3 interpolate(float3 p0, float3 p1, float v0, float v1) {
  float mu;
  float3 p;

  if (v0 == 1.0f)
    return p0;
  if (v1 == 1.0f)
    return p1;
  if ((v1 - v0) == 0.0f)
    return p0;
  mu = (1.0f - v0) / (v1 - v0);
#ifdef __APPLE__
  p = p0 + mu * (p1 - p0);
#else
  p = VEC3_INTERPOLATE(p0, mu, p1);
#endif
  return p;
}

static float3 *get_vertices(uint cubeindex, float3 *v_pos, float *v_val,
                            uint pos) {
  float3 *vertlist;

  if (!(vertlist = (float3 *)malloc(12 * sizeof(float3))))
    return NULL;

  if (edgetable[cubeindex] & 1)
    vertlist[0] = interpolate(v_pos[pos + 0], v_pos[pos + 1], v_val[pos + 0],
                              v_val[pos + 1]);
  if (edgetable[cubeindex] & 2)
    vertlist[1] = interpolate(v_pos[pos + 1], v_pos[pos + 2], v_val[pos + 1],
                              v_val[pos + 2]);
  if (edgetable[cubeindex] & 4)
    vertlist[2] = interpolate(v_pos[pos + 2], v_pos[pos + 3], v_val[pos + 2],
                              v_val[pos + 3]);
  if (edgetable[cubeindex] & 8)
    vertlist[3] = interpolate(v_pos[pos + 3], v_pos[pos + 0], v_val[pos + 3],
                              v_val[pos + 0]);
  if (edgetable[cubeindex] & 16)
    vertlist[4] = interpolate(v_pos[pos + 4], v_pos[pos + 5], v_val[pos + 4],
                              v_val[pos + 5]);
  if (edgetable[cubeindex] & 32)
    vertlist[5] = interpolate(v_pos[pos + 5], v_pos[pos + 6], v_val[pos + 5],
                              v_val[pos + 6]);
  if (edgetable[cubeindex] & 64)
    vertlist[6] = interpolate(v_pos[pos + 6], v_pos[pos + 7], v_val[pos + 6],
                              v_val[pos + 7]);
  if (edgetable[cubeindex] & 128)
    vertlist[7] = interpolate(v_pos[pos + 7], v_pos[pos + 4], v_val[pos + 7],
                              v_val[pos + 4]);
  if (edgetable[cubeindex] & 256)
    vertlist[8] = interpolate(v_pos[pos + 0], v_pos[pos + 4], v_val[pos + 0],
                              v_val[pos + 4]);
  if (edgetable[cubeindex] & 512)
    vertlist[9] = interpolate(v_pos[pos + 1], v_pos[pos + 5], v_val[pos + 1],
                              v_val[pos + 5]);
  if (edgetable[cubeindex] & 1024)
    vertlist[10] = interpolate(v_pos[pos + 2], v_pos[pos + 6], v_val[pos + 2],
                               v_val[pos + 6]);
  if (edgetable[cubeindex] & 2048)
    vertlist[11] = interpolate(v_pos[pos + 3], v_pos[pos + 7], v_val[pos + 3],
                               v_val[pos + 7]);
  return vertlist;
}

static float3 **package_triangles(float3 *vertlist, uint cubeindex, uint i,
                                  t_data *data) {
  uint2 len;
  float3 **tris_new;

  len.x = 0;
  len.y = 1;
  tris_new = NULL;
  if (!(tris_new = alloc_float3_arr(tris_new, &len)))
    error(MALLOC_FAIL_ERR, data);

  tris_new[0][0] = vertlist[tritable[cubeindex][i]];
  tris_new[0][1] = vertlist[tritable[cubeindex][i + 1]];
  tris_new[0][2] = vertlist[tritable[cubeindex][i + 2]];
  return tris_new;
}

float3 **polygonise(float3 *v_pos, float *v_val, uint2 *pos, t_data *data) {
  float3 **tris;
  float3 **tris_new;
  float3 *vertlist;
  uint cubeindex;
  uint i;
  uint2 len;

  tris = NULL;
  tris_new = NULL;
  vertlist = NULL;
  i = 0;
  len.x = 0;
  cubeindex = getCubeIndex(v_val, pos->x);
  if (edgetable[cubeindex] == 0)
    return NULL;
  if (!(vertlist = get_vertices(cubeindex, v_pos, v_val, pos->x)))
    error(MALLOC_FAIL_ERR, data);

  while ((int)tritable[cubeindex][i] != -1) {
    tris_new = package_triangles(vertlist, cubeindex, i, data);
    len.y = 1;
    if (!(tris = arr_float3_cat(tris_new, tris, &len)))
      error(MALLOC_FAIL_ERR, data);
    i += 3;
  }
  data->len.y = len.x;

  free(vertlist);
  return tris;
}
