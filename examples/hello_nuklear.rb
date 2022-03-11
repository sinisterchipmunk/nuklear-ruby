#!/usr/bin/env ruby
#
# This example is completely self-contained (the files in /examples/lib/ are
# not used), so it makes a good "holistic" reference. It's a little on the
# large side, mainly because of the SDL2 event loop as interesting events have
# to be delegated into Nuklear so that, for example, button clicks actually
# do something.
#
# This example also eschews the Nuklear DSL, instead preferring to instantiate
# the UI widgets directly. Essentially, this shows you what is happening
# within the DSL, and it can be a useful reference if you're in a situation
# where the DSL is cumbersome to use. That said, it's not nearly as pretty
# or intuitive as using the DSL would be.
#
require 'bundler/setup'
require 'sdl2'
require 'nuklear'
require 'opengl'
include OpenGL
OpenGL.load_lib()

module SDL2::Window::Flags
  # Shim: Older versions of SDL2 for Ruby didn't define this flag.
  ALLOW_HIGHDPI = 0x00002000 unless defined?(SDL2::Window::Flags::ALLOW_HIGHDPI)
end

# Initialize SDL2 and OpenGL.
SDL2.init(SDL2::INIT_EVERYTHING)
SDL2::GL.set_attribute(SDL2::GL::ACCELERATED_VISUAL, 1)
window = SDL2::Window.create "Nuklear", 0, 0, 640, 480,
                              SDL2::Window::Flags::OPENGL |
                              SDL2::Window::Flags::ALLOW_HIGHDPI |
                              SDL2::Window::Flags::RESIZABLE
@gl = SDL2::GL::Context.create(window)

# When Nuklear loads the font file it will yield to us the width, height
# and baked font atlas image data. We must then tell OpenGL to load that
# image data into a texture ID, and then pass the OpenGL texture ID back into
# Nuklear for later reference.
font = Nuklear::Font.new(File.expand_path("arial.ttf", __dir__), 14) do |w, h, data|
  # give the image data to OpenGL
  tex_names_buf = ' ' * 8
  glGenTextures(1, tex_names_buf)
  texture_id = tex_names_buf.unpack('L1')[0]
  glBindTexture(GL_TEXTURE_2D, texture_id)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
  # give the GL texture ID to Nuklear
  texture_id
end

ctx = Nuklear::Context.new(font)

# We'll use the OpenGL 2 renderer for Nuklear. Notice that we must tell the
# renderer the size of the window and the size of the drawing surface, which
# aren't always the same thing.
require 'nuklear/nuklear_renderer_opengl2'
ctx.renderer = Nuklear::Renderer::OpenGL2.new
ctx.renderer.window_size = window.size
ctx.renderer.drawable_size = window.gl_drawable_size

# We can now build our Nuklear UI. Let's create a window, then add a few
# labels and a button to it.
@window = Nuklear::UI::Window.new("Hello Nuklear",
                                  left: 60, top: 60, width: 300, height: 200,
                                  border: true, scalable: true, movable: true, minimizable: true)
@window.layout_row_dynamic 35, 2
@window << Nuklear::UI::Label.new("This is a label")
@window << Nuklear::UI::Button.new("This is a button")
@window << Nuklear::UI::Label.new("Frame number:")
@window << @frame_num_label = Nuklear::UI::Label.new("0")
# Let's also add a large, empty container to the window. This forces Nuklear
# to add a scrollbar to the window.
@window.layout_row_dynamic 200, 2
container = Nuklear::UI::Group.new
@window << container

# Finally, add the window to the context so that Nuklear will render it. We
# can add any number of windows.
ctx << @window

# Now for the main event loop. This loop is almost entirely dedicated to event
# processing. Below the event processing, we'll deal with Nuklear.
@frame_count = 0
glViewport(0, 0, *window.gl_drawable_size)
glClearColor(0, 0, 0, 0)
@last_tick_time = Time.now
while true
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

  while event = ::SDL2::Event.poll
    case event
    # TODO touch events
    # when SDL2::Event::FingerMotion
    # when SDL2::Event::FingerDown
    # when SDL2::Event::FingerUp
    when ::SDL2::Event::TextInput   then ctx.trigger(:glyph,  event.text)
    when ::SDL2::Event::MouseMotion then ctx.trigger(:motion, event.x, event.y, event.xrel, event.yrel)
    when ::SDL2::Event::MouseWheel  then ctx.trigger(:scroll, event.x, event.y)
    when ::SDL2::Event::MouseButtonDown, ::SDL2::Event::MouseButtonUp
      is_down = event.kind_of?(::SDL2::Event::MouseButtonDown)
      if event.button == 1 and event.clicks > 1
        ctx.trigger(:button, :double, event.x, event.y, is_down)
      else
        case event.button
        # SDL_BUTTON_LEFT seems to be not defined
        when 1 then ctx.trigger(:button, :left,   event.x, event.y, is_down)
        when 2 then ctx.trigger(:button, :middle, event.x, event.y, is_down)
        when 3 then ctx.trigger(:button, :right,  event.x, event.y, is_down)
        end
      end
    when ::SDL2::Event::Window
      case event.event
      when ::SDL2::Event::Window::FOCUS_GAINED then @focused = true
      when ::SDL2::Event::Window::FOCUS_LOST   then @focused = false
      end
    when ::SDL2::Event::KeyDown, ::SDL2::Event::KeyUp
      is_down = event.kind_of?(::SDL2::Event::KeyDown)
      case event.sym
      when ::SDL2::Key::LSHIFT, ::SDL2::Key::RSHIFT then ctx.trigger(:key, :shift,           is_down)
      when ::SDL2::Key::DELETE                      then ctx.trigger(:key, :del,             is_down)
      when ::SDL2::Key::RETURN                      then ctx.trigger(:key, :enter,           is_down)
      when ::SDL2::Key::TAB                         then ctx.trigger(:key, :tab,             is_down)
      when ::SDL2::Key::BACKSPACE                   then ctx.trigger(:key, :backspace,       is_down)
      when ::SDL2::Key::HOME                        then ctx.trigger(:key, :text_start,      is_down)
                                                         ctx.trigger(:key, :scroll_start,    is_down)
      when ::SDL2::Key::END                         then ctx.trigger(:key, :text_end,        is_down)
                                                         ctx.trigger(:key, :scroll_end,      is_down)
      when ::SDL2::Key::PAGEDOWN                    then ctx.trigger(:key, :scroll_down,     is_down)
      when ::SDL2::Key::PAGEUP                      then ctx.trigger(:key, :scroll_up,       is_down)
      when ::SDL2::Key::Z                           then ctx.trigger(:key, :text_undo,       is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::R                           then ctx.trigger(:key, :text_redo,       is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::C                           then ctx.trigger(:key, :copy,            is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::V                           then ctx.trigger(:key, :paste,           is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::X                           then ctx.trigger(:key, :cut,             is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::B                           then ctx.trigger(:key, :text_line_start, is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::E                           then ctx.trigger(:key, :text_line_end,   is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
      when ::SDL2::Key::UP                          then ctx.trigger(:key, :up,              is_down)
      when ::SDL2::Key::DOWN                        then ctx.trigger(:key, :down,            is_down)
      when ::SDL2::Key::LEFT
        if event.mod & ::SDL2::Key::Mod::CTRL > 0   then ctx.trigger(:key, :text_word_left,  is_down)
        else ctx.trigger(:key, :left, is_down)
        end
      when ::SDL2::Key::RIGHT
        if event.mod & ::SDL2::Key::Mod::CTRL > 0   then ctx.trigger(:key, :text_word_right, is_down)
        else ctx.trigger(:key, :right, is_down)
        end
      end

    when SDL2::Event::Quit then exit
    when SDL2::Event::KeyDown
      case event.sym
      when SDL2::Key::C
        if (event.mod & SDL2::Key::Mod::CTRL) > 0
          exit
        end
      end
    end
  end

  # Track the number of frames and calculate the amount of time that has
  # elapsed between the last frame and this one.
  @frame_count += 1
  time = Time.now
  delta ||= begin
    delta = time - @last_tick_time
  end
  @last_tick_time = time

  # Update the frame counter label. The next time Nuklear renders it, the
  # label will contain the updated text.
  @frame_num_label.text = @frame_count.to_s

  # Update the Nuklear context. This will generate/update the command buffer
  # in preparation for rendering, then pass that command buffer to our
  # renderer.
  ctx.tick

  # Show the rendered frame.
  window.gl_swap
end
