require 'opengl'
include OpenGL
OpenGL.load_lib()
SDL2::GL.set_attribute(SDL2::GL::ACCELERATED_VISUAL, 1)
