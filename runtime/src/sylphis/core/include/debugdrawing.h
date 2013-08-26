
#ifndef DEBUGDRAWING
#define DEBUGDRAWING

#include <platform/types.h>

void setTransform (const F32 pos[3], const F32 R[12]);
void drawBox(const F32 pos[3], const F32 R[12], const F32 sides[3]);


#endif 
