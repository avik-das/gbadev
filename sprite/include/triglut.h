#ifndef TRIG_LUT_H
#define TRIG_LUT_H

#define INLINE static inline

extern const s16 sin_lut[514];

// Sine and cosine lookups. Theta's range is [0, 0xFFFF] for [0,2π), just like
// in the BIOS functions.

INLINE s32 lu_sin(u32 theta) { return sin_lut[( theta >> 7       ) & 0x1FF]; }
INLINE s32 lu_cos(u32 theta) { return sin_lut[((theta >> 7) + 128) & 0x1FF]; }

#endif
