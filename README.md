# Paradox3D - Software based 3D renderer and game engine

This project is on hold. It's provided "as-is" and further development is uncertain.

The code is incomplete and quite messy. However, you can use it as an example or to boostrap your own software-based renderer and/or 3D game engine.

It's all coded from scratch (with minor exceptions) in plain C.<br>
It requires SDL2 to create an OS-agnostic rendering context and handle input events.<br>
You can code those features yourself utilizing the provided APIs of your target OS if you prefer to ditch the 3rd party deptendency.

## Compiling
Currently it's configured to be build with Visual Studio on Windows and gcc on macOS or Linux.<br>
* When using the Makefile (not building with Visual Studio) make sure to set your SDL2 paths before building.<br>
* When using Visual Studio, again - make sure you set the SDL2 paths in the project properties include and lib directories (the VS linker and compiler options).

## Some of the features include:

- perspective-correct texture mapping with barycentric interpolation
- bresenham's line drawing algorithm
- realtime illumination
- own 3D rendering engine with z-buffer
- the rendering context is provided by SDL2
- supports bacface-culling
- supports frustum polygon clipping
- the input is handled by SDL2
- supported 3D models are OBJ format
- supported texture format is PNG
- there is no spatial partitioning at this point

![Screenshot 2024-04-03 at 13 55 50](https://github.com/jeuxdemains/Paradox3D-software/assets/7083803/4303b5d9-aa2e-4e8c-b52e-2fcf36ccc89c)

Tech Demo:
https://youtu.be/mdugjHo4CDM?si=DkSsutngI7AYFAr8
