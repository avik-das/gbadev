#include "graphics.h"

OBJATTR OBJ_BUFFER[128];
OBJAFFINE * const OBJ_AFF_BUFFER = (OBJAFFINE *) OBJ_BUFFER;

void oam_copy(OBJATTR *dst, OBJATTR *src, unsigned int count) {
    // Struct copying would be the right approach:
    //
    //   while(count--) {
    //       *dst++ = *src++;
    //   }
    //
    // However, a bug in DevkitPro makes this approach not work. Fall back to
    // word-by-word copies instead.

    u32 *dstw = (u32*) dst;
    u32 *srcw = (u32*) src;
    while (count--) {
        *dstw++ = *srcw++;
        *dstw++ = *srcw++;
    }
}

void oam_init(OBJATTR *obj, unsigned int count) {
    u32 nn = count;
    u32 *dst = (u32 *) obj;

    while (nn--) {
        *dst++ = OBJ_DISABLE;
        *dst++ = 0;
    }

    oam_copy(OAM, obj, count);
}

void obj_aff_rotation(OBJAFFINE *oaff, u16 alpha) {
    int ss = lu_sin(alpha) >> 4;
    int cc = lu_cos(alpha) >> 4;

    oaff->pa =  cc;
    oaff->pb = -ss;
    oaff->pc =  ss;
    oaff->pd =  cc;
}
