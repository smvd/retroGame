#ifndef INCLUDED_BITWISE_HEADER

#define GET_BIT(p, b) (((p) & (1 << (b))) != 0)
#define SET_BIT(p, b) ((p) |= (1 << (b)))
#define CLR_BIT(p, b) ((p) &= ~(1 << (b)))

#endif
