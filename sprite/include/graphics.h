#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gba.h>
#include "triglut.h"

// GRAPHICS SUBSYSTEM
//
// While libgba provides most of the necessary data types and functions for
// accessing memory, a few more typedefs make it easier to set up the graphics
// subsystem, as well as update it.
//
// These definitions and implementations are adapted from TONC. See
// http://www.coranac.com/tonc/text/toc.htm for details.

typedef struct { u32 data[ 8]; } TILE, TILE4;
typedef struct { u32 data[16]; } TILE8;

typedef TILE  CHARBLOCK [512];
typedef TILE8 CHARBLOCK8[256];

#define TILE_MEM   ((CHARBLOCK  *) VRAM)
#define TILE8_MEM  ((CHARBLOCK8 *) VRAM)

extern OBJATTR OBJ_BUFFER[128];
extern OBJAFFINE * const OBJ_AFF_BUFFER;

void oam_copy(OBJATTR *dst, OBJATTR *src, unsigned int count);
void oam_init(OBJATTR *obj, unsigned int count);
void obj_aff_rotation(OBJAFFINE *oaff, u16 alpha);

#endif
