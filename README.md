# Paradox3D - Software based 3D renderer and game engine

This project is incomplete and rather a draft than anything else. It was meant to (and still is) grow to a full-blown retro FPS game engine.

Read the code with a grain of salt as it's not well refactored nor complete for that matter.

It's all coded from scratch (with minor exceptions) in plain C.



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
