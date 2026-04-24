#ifndef INCLUDED_VECTOR_HEADER
#define INCLUDED_VECTOR_HEADER

#include <math.h>

struct _Vector {
    float x;
    float y;
};

struct _Vector VECTOR_Rotate(struct _Vector in, float angle);

#endif
