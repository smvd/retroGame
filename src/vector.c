#include "vector.h"

struct _Vector VECTOR_Rotate(struct _Vector in, float angle) {
    struct _Vector out;
    
    out.x = in.x * cos(angle) - in.y * sin(angle);
    out.y = in.x * sin(angle) + in.y * cos(angle);
    
    return out;
}
