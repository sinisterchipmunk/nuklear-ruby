require 'fiddle/import'

module Nuklear
  # This base class provides a pure-Ruby renderer implementation for Nuklear.
  # Importantly, it doesn't actually *draw* anything. Instead it provides a
  # generic renderer implementation where subclasses need only override the
  # #draw method to draw something.
  #
  # In most cases, subclasses will also have to override #initialize to set up
  # one-time state (e.g. compiling shaders), and also #render to set up
  # per-frame state (e.g. activating shaders). In both cases, subclasses
  # should call `super`.
  #
  class Renderer
    attr_accessor :window_size
    attr_writer :drawable_size

    # Returns the configuration necessary for converting a Nuklear rendering
    # pass into a renderer-specific one. This data can be changed at run-time;
    # it is not cached.
    attr_accessor :convert_config

    # Assign the handle for a 1x1 white texture to this accessor. It doesn't
    # matter what format this takes, (GL texture ID, memory address of texture
    # data, etc), as long as the renderer can decode this into a usable result
    # when it's passed back to it later on.
    attr_reader :null_texture_handle

    # Nuklear::Buffer containing vertex data
    attr_reader :vertices

    # Nuklear::Buffer containing vertex index data
    attr_reader :vertex_indices

    # Nuklear::Buffer containing command data
    attr_reader :commands

    # Nuklear::Context for this renderer
    attr_reader :context

    Vertex = Fiddle::Importer.struct [
      'float position[3]',
      'float uv[3]',
      'unsigned char color[4]'
    ]

    # For all renderer implementations, override #initialize to set up your
    # initial state (e.g. compiling shaders) and call `super`.
    def initialize(context)
      @commands = Nuklear::Buffer.new
      @vertices = Nuklear::Buffer.new
      @vertex_indices = Nuklear::Buffer.new
      @context = context
      @window_size = [640, 480]
      @drawable_size = nil
      @null_texture_handle = 0
      @convert_config = {
        vertex_layout: [
          [:position, :float,    Vertex.offsetof('position')],
          [:texcoord, :float,    Vertex.offsetof('uv')],
          [:color,    :r8g8b8a8, Vertex.offsetof('color')]
        ],
        vertex_size: Vertex.size,
        vertex_alignment: Vertex.alignment,
        null: null_texture_handle,
        circle_segment_count: 22,
        curve_segment_count: 22,
        arc_segment_count: 22,
        global_alpha: 1.0,
        shape_aa: true,
        line_aa: true
      }
    end

    def null_texture_handle=(n)
      @null_texture_handle = n
      @convert_config[:null] = n
    end

    def drawable_size
      @drawable_size || window_size
    end

    # Called at the start of a render pass. Renderers may wish to override
    # this method to apply once-per-frame state updates. They should call
    # `super` when they are ready to proceed.
    def render
      nk_convert
      nk_draw_foreach { |command| draw(command) }
    end

    # Called for each Nuklear draw command, numerous times per render pass.
    # Renderers should override this method to actually draw something, but
    # keep this method as lightweight as possible.
    #
    # `element_count`: The number of elements to be drawn.
    # `clip_rect`: An array containing the x, y, width and height
    #              (in that order) of the region to be drawn to.
    # `texture_handle`: The handle of a texture to be drawn. See
    #                   #null_texture_handle.
    # `offset`: The number of elements already drawn, not including the
    #           current command.
    #
    def draw(cmd)

    end
  end
end
