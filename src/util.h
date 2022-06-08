#ifndef UTIL_H
#define UTIL_H

#include "vector.h"

char *util_read_file(const char *fp);

float util_restrict_angle(float angle);

// Returns distance along vector p with direction r
float util_rays_intersection(Vec2f p, Vec2f r, Vec2f q, Vec2f s);

#endif

