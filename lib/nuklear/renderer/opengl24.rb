module Nuklear
  class Renderer
    def self.renderer_class
      if SDL2::GL.get_attribute(SDL2::GL::CONTEXT_MAJOR_VERSION) > 2
        require 'nuklear_renderer_opengl4'
        Nuklear::Renderer::OpenGL4
      else
        require 'nuklear_renderer_opengl2'
        Nuklear::Renderer::OpenGL2
      end
    end
  end
end
