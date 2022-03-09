#!/usr/bin/env ruby
require 'bundler/setup'
require 'sdl2'
require 'nuklear'
require 'nuklear/input/sdl2'
require 'opengl'
include OpenGL
OpenGL.load_lib()
include Nuklear::Input

class Font
  attr_reader :path

  def initialize(path, size = 12)
    @path = path
    @size = size
    @texture_id = nil
    @nuklear_handle = nil
  end

  def texture_id
    @texture_id ||= begin
      tex_names_buf = ' ' * 8
      glGenTextures(1, tex_names_buf)
      tex_names_buf.unpack('L1')[0]
    end
  end

  def nuklear_handle
    @nuklear_handle ||= Nuklear::Font.new(path, @size) do |w, h, data|
      glBindTexture(GL_TEXTURE_2D, texture_id)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
      texture_id
    end
  end
end

module SDL2::Window::Flags
  ALLOW_HIGHDPI = 0x00002000 unless defined?(SDL2::Window::Flags::ALLOW_HIGHDPI)
end

SDL2.init(SDL2::INIT_EVERYTHING)
SDL2::GL.set_attribute(SDL2::GL::ACCELERATED_VISUAL, 1)
# SDL2::GL.set_attribute(SDL2::GL::CONTEXT_MAJOR_VERSION, 4)
# SDL2::GL.set_attribute(SDL2::GL::CONTEXT_MINOR_VERSION, 5)
window = SDL2::Window.create "Nuklear", 0, 0, 640, 480,
                              SDL2::Window::Flags::OPENGL |
                              SDL2::Window::Flags::ALLOW_HIGHDPI |
                              SDL2::Window::Flags::RESIZABLE
@gl = SDL2::GL::Context.create(window)

font = Font.new File.expand_path("arial.ttf", __dir__)
ctx = Nuklear::Context.new(font.nuklear_handle)
ctx.input do |input|
  sdl2(input, true) do |event|
    case event
    when SDL2::Event::Quit then exit
    when SDL2::Event::KeyDown
      case event.sym
      when SDL2::Key::C
        if (event.mod & SDL2::Key::Mod::CTRL) > 0
          exit
        end
      end
    # else p event
    end
  end
end

require 'nuklear/nuklear_renderer_opengl2'
ctx.renderer = Nuklear::Renderer::OpenGL2.new(ctx)
ctx.renderer.window_size = window.size
ctx.renderer.drawable_size = window.gl_drawable_size

@window = Nuklear::UI::Window.new("Hello Nuklear",
                                  left: 60, top: 60, width: 300, height: 200,
                                  border: true, scalable: true, movable: true, minimizable: true)
@window.layout_row_dynamic 35, 2
@window << Nuklear::UI::Label.new("This is a label")
@window << Nuklear::UI::Button.new("This is a button")
@window << Nuklear::UI::Label.new("Frame number:")
@window << @frame_num_label = Nuklear::UI::Label.new("0")
@window.layout_row_dynamic 200, 2
container = Nuklear::UI::Group.new
@window << container
ctx << @window

@frame_count = 0
glViewport(0, 0, *window.gl_drawable_size)
glClearColor(0, 0, 0, 0)
@last_tick_time = Time.now
while true
  @frame_count += 1
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
  
  time = Time.now
  delta ||= begin
    delta = time - @last_tick_time
  end
  @last_tick_time = time

  @frame_num_label.text = @frame_count.to_s

  ctx.tick
  remaining = ctx.commands.dup
  while remaining.any?
    cmd = remaining.shift
    cmd.tick(delta) if cmd.respond_to?(:tick)
    remaining.concat cmd.commands if cmd.respond_to?(:commands)
  end
  window.gl_swap
end
