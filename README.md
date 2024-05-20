# Paradox3D - Software based 3D renderer and game engine

This project is on hold. It's provided "as-is" and further development is uncertain.

The code is incomplete and quite messy. However, you can use it as an example or to boostrap your own software-based renderer and/or 3D game engine.

The rendering (vector & matrix math, line-drawing algo, rasterization, texture mapping, polygon splitting, frustum clipping, etc.) is all coded from scratch in plain C.<br>
It requires SDL2 to create an OS-agnostic rendering context and handle input events.<br>
You can implement a native one, utilizing the provided APIs of your target OS if you prefer to ditch the 3rd party deptendency.

## Compiling
Currently it's configured to be build with Visual Studio on Windows and gcc on macOS or Linux.<br>
* When using the Makefile (not building with Visual Studio) make sure to set your SDL2 paths before building.<br>
* When using Visual Studio, again - make sure you set the SDL2 paths in the project properties include and lib directories (the VS linker and compiler options).

## Some of the features include:

- perspective-correct texture mapping via barycentric interpolation
- bresenham's line drawing algorithm to perform primitives rendering
- scanline rasterization
- realtime illumination
- 3D math (3D transformations, matrix and vector operations)
- z-buffer
- backface-culling
- frustum polygon clipping
- mouse and keyboard input
- supports 3D models in OBJ format
- supports textures in PNG format
- no spatial partitioning at this point

![Screenshot 2024-04-03 at 13 55 50](https://github.com/jeuxdemains/Paradox3D-software/assets/7083803/4303b5d9-aa2e-4e8c-b52e-2fcf36ccc89c)

Tech Demo:
https://youtu.be/mdugjHo4CDM?si=DkSsutngI7AYFAr8
