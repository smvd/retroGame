#ifndef INCLUDED_FX_HEADER
#define INCLUDED_FX_HEADER

#include <stdint.h>

typedef int32_t fx32_t;     // 16.16 fixed

#define FX_SHIFT 16
#define TO_FX(x)   ((fx32_t)((x) << FX_SHIFT))
#define FROM_FX(x)   ((uint16_t)(x >> FX_SHIFT))
#define MUL(a,b)  ((fx32_t)(((int64_t)(a) * (b)) >> FX_SHIFT))
#define DIV(a,b)  ((fx32_t)(((int64_t)(a) << FX_SHIFT) / (b)))
#define ABS(v) ((v) < 0 ? -(v) : (v))

struct _FX_Vector {
    fx32_t x;
    fx32_t y;
};

struct _Vector {
    uint16_t x;
    uint16_t y;
};

#endif
