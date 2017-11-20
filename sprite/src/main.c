#include <gba.h>
#include <string.h>

#include "graphics.h"

#include "ghost.h"
#include "cloud.h"

// Like the tile data, this map could have been generated ahead of time.
// However, the background map is very repetitive, so it's easy to generate at
// initialization time.
const unsigned int bgMapLen = 2048;
unsigned short bgMap[1024];

int main() {
    // Enable the interrupt request for V-Blank. This way, we can sleep until
    // the end of a frame, instead of using up battery power.
    irqInit();
    irqEnable(IRQ_VBLANK);

    // Set the key repeat parameters. When a key is held down, there will be a
    // delay, after which the key will be reported as newly-pressed at regular
    // intervals.
    //
    // Provided by libgba.
    setRepeat(5, 2);

    // Configure the graphics subsystem:
    //
    // 1. Mode 0 gives us four regular backgrounds, in tiled mode.
    // 2. Only one of the backgrounds is enabled.
    // 3. Sprites are enabled.
    // 4. Sprite tiles are read in 1D mode, meaning consecutive tiles used to
    //    construct a larger sprite appear contiguously in memory.
    REG_DISPCNT = MODE_0 | BG0_ON | OBJ_ENABLE | OBJ_1D_MAP;

    // Set up the ghost sprite. Start by copying over the tile and palette data
    // to the respective locations in memory. The source data comes from having
    // processed the PNG source images using "grit".
    memcpy(TILE8_MEM[4], ghostTiles, ghostTilesLen);
    memcpy(SPRITE_PALETTE, ghostPal, ghostPalLen);

    // Zero out the OAM buffer, and hold onto parts of the buffer so we can
    // update them in the main loop. Note that the object affine buffer is
    // actually the object buffer, but viewed using a different struct.
    oam_init(OBJ_BUFFER, 128);
    OBJATTR *spr0 = &OBJ_BUFFER[0];
    OBJAFFINE *spraff0 = &OBJ_AFF_BUFFER[0];

    // Initialize some parameters related to the sprite that will be updated
    // during the main loop.
    int x = 96;
    int y = 32;
    u32 animation_frame = 0;
    u32 alpha = 0;

    // Initialize the background. Start by constructing the map. The map
    // consists of six tiles. The first three tiles are repeated in the first
    // line, then the next three are repeated in the next line.
    for (int y = 0; y < 32; y++) {
        u16 base = y % 2 ? 0x3 : 0x0;
        for (int x = 0; x < 32; x++) {
            bgMap[y * 32 + x] = base + (x % 3);
        }
    }

    // Copy over the background tile, palette and map data to their respective
    // locations in memory.
    memcpy(TILE8_MEM[0], cloudTiles, cloudTilesLen);
    memcpy(BG_PALETTE, cloudPal, cloudPalLen);
    memcpy(&MAP[30][0], bgMap, bgMapLen);

    // Configure the only background being used:
    //
    // 1. The tiles come from the first tile block.
    // 2. The map comes from the 30th screen block.
    // 3. The background is 16-colors, i.e. 4 bits per pixel.
    // 4. The background is 256x256.
    // 5. The background is not offset by any amount.
    REG_BG0CNT =
        CHAR_BASE(0) | SCREEN_BASE(30) | BG_16_COLOR | TEXTBG_SIZE_256x256;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;

    // The main loop. Never ends!
    while (1) {
        // Start by polling the keys. Here, libgba provides some helpful
        // utilities, taking care of debouncing and key repetition.
        scanKeys();
        u16 keys_repeat = keysDownRepeat();

        // The D-pad moves the sprite around, but only within the boundaries of
        // the screen. Note that the sprite's boundaries are double-sized, as
        // mentioned in the affine transformation section below, so that
        // affects what x- and y-coordinates map to the extreme edges of the
        // screen.
        //
        // Keep track of if the sprite has moved this frame, because if so, the
        // sprite needs to animate.
        int has_moved = 0;
        if (keys_repeat & KEY_LEFT  && x >= -6 ) { x -= 2; has_moved = 1; }
        if (keys_repeat & KEY_RIGHT && x <= 214) { x += 2; has_moved = 1; }
        if (keys_repeat & KEY_UP    && y >= -6 ) { y -= 2; has_moved = 1; }
        if (keys_repeat & KEY_DOWN  && y <= 134) { y += 2; has_moved = 1; }

        // The shoulder buttons rotate the sprite.
        if (keys_repeat & KEY_L) { alpha += 0x7FF; }
        if (keys_repeat & KEY_R) { alpha -= 0x7FF; }

        // By moving, the animation will be kicked off. However, even if the
        // sprite has not moved, if the animation has started, it should be
        // continued until it ends.
        if (has_moved || animation_frame) {
            animation_frame = (animation_frame + 1) & 0x3F;
        }
        // Every few frames, the tile changes, creating a visual change in the
        // sprite. This makes sure the animation doesn't happen too quickly,
        // i.e. the animation happens every couple of frames, not every frame.
        int tile_id = (animation_frame >> 2) & 0xC;

        // Now, construct the sprite attributes:
        //
        // 1. The sprite is square and 16x16.
        // 2. It is 4 bits per pixel.
        // 3. It is an affine sprite, with a double-sized rendering window.
        //    This ensures that, when rotating, if the sprite's pixels goes
        //    outside the use 16x16 rendering window, it will still be
        //    rendered.
        // 4. The affine matrix used for the sprite is the first one.
        // 5. The sprite uses the first palette.
        // 6. It uses the tile computed above.
        // 7. It is at the x- and y-coordinates computed above.
        spr0->attr0 =
            ATTR0_SQUARE |
            ATTR0_COLOR_16 |
            ATTR0_ROTSCALE_DOUBLE |
            OBJ_Y(y);
        spr0->attr1 = ATTR1_SIZE_16 | ATTR1_ROTDATA(0) | OBJ_X(x);
        spr0->attr2 = ATTR2_PALETTE(0) | tile_id;

        // Next, update the affine matrix for the sprite, given the rotation
        // angle computed above.
        obj_aff_rotation(spraff0, alpha);

        // All our computation from this frame is done. Before we can render
        // the updated graphics, we have to wait for the OAM to unlock. Even if
        // that memory were not locked, we would not want to update the
        // graphics while they were rendering.
        //
        // Thus, we wait for the last frame to finish rendering by waiting for
        // the V-Blank interrup.
        VBlankIntrWait();

        // Finally, copy over the OAM data in our buffer to the actual OAM. The
        // entire buffer doesn't need to be copied, but it's easier to just
        // copy everything.
        oam_copy(OAM, OBJ_BUFFER, 128);
    }
}
