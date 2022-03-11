require_relative 'sdl2_input'
require 'nuklear/nuklear_renderer_opengl2'

module Examples
  class Window
    include Examples::SDL2Input
    attr_reader :nuklear_context

    def initialize(title = "Example", left: 0, top: 0, width: 640, height: 480, hidden: false)
      flags = SDL2::Window::Flags::OPENGL |
              SDL2::Window::Flags::ALLOW_HIGHDPI |
              SDL2::Window::Flags::RESIZABLE
      flags = flags | SDL2::Window::Flags::HIDDEN if hidden
      @handle = SDL2::Window.create title, left, top, width, height, flags
      @gl = SDL2::GL::Context.create(@handle)
      @focused = false
      glViewport(0, 0, *renderable_size)
      glClearColor(0, 0, 0, 0)
    end

    def close
      @handle.destroy
    end

    def frame
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
      process_sdl2_events(@nuklear_context)
      yield # do app specific stuff
      @handle.gl_swap
    end

    def focused?
      @focused
    end

    def nuklear_context=(ctx)
      @nuklear_context = ctx
      @nuklear_context.renderer = Nuklear::Renderer::OpenGL2.new
      @nuklear_context.renderer.window_size = @handle.size
      @nuklear_context.renderer.drawable_size = renderable_size
    end

    def renderable_size
      @handle.gl_drawable_size
    end
  end
end
