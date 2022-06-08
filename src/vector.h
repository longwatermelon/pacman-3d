#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x, y;
} Vec2f;

float vec_len(Vec2f a);

Vec2f vec_addv(Vec2f a, Vec2f b);
// a - b
Vec2f vec_subv(Vec2f a, Vec2f b);
Vec2f vec_mulf(Vec2f a, float t);

float vec_cross(Vec2f a, Vec2f b);

#endif

