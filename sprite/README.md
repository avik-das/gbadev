Sprite
======

A simple test program that demonstrates a wide variety of hardware-provided
functionality and general programming techniques.

![Screen capture of sprite program, demonstrating a character moving around and rotating in reaction to user input](screenshot-1.gif?raw=true)

Running
-------

Make sure you have `grit` available on your `PATH`. See the [main README](../README.md#building-and-installing-grit) for more information.

To assemble and run `sprite` using an emulator like VBA-M:

```sh
make
vbam sprite.gba
```

The following buttons are utilized:

- **D-pad**: move the sprite
- **A**: switch the sprite palette
- **B**: flip sprite vertically
- **SELECT**: switch background priority
- **START**: toggle the window layer

Functionality
-------------

The following hardware-provided functionality is demonstrated by this program:

* A means of creating an entire Game Boy Advance program, suitable for use with any compliant GBA emulator. The program can be used as a starting point for more interesting demos.

* Display of sprites and backgrounds.

* Use of the affine transformation matrix to rotate a sprite.

* Reaction to user input. Demonstrates how user input can be used to affect a visible sprite.

The following programming techniques are demonstrated by this program:

* Movement of a sprite in response to user input, with the rate of movement controlled to provide a smooth experience for the user.

* Collision detection with the edges of the screen.

* Animation of a sprite involving multiple frames, with a deliberately limited frame-rate, and a rest frame to which the animation terminates if the sprite is not moving. This demonstrates how to limit the frame-rate, as well as how to work with an implicit state machine in order to provide animation.

* Use of a pre-computed lookup table to calculate trigonometric functions in response to user input.

* Use of a V-Blank interrupt to reduce the power consumption, as looping until V-Blank would be CPU intensive.

Further reading
---------------

This program serves as my first step into GBA development, and as such, the credit goes to the wealth of information available online. By far the most thorough and useful resource is [TONC](https://www.coranac.com/tonc/text/toc.htm).
