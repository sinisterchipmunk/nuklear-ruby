require "test_helper"

class TexturesTest < Minitest::Test
  EXPECTED = 0xFFFFFFFFFFFFFFFF

  class TextureTestRenderer < Nuklear::Renderer
    def initialize(&callback)
      super
      convert_config[:null] = Fiddle::Pointer.new(EXPECTED)
      @callback = callback
    end

    def draw(cmd)
      @callback&.call cmd
    end
  end

  def test_64bit_texture_pointer_addresses
    all_cmds = []
    window = Examples::Window.new "Calculator Example", hidden: true
    font_path = File.expand_path("../examples/arial.ttf", __dir__)
    ctx = Nuklear::Context.new Nuklear::Font.new(font_path, 12) { Fiddle::Pointer.new(EXPECTED) }
    window.nuklear_context = ctx
    window.nuklear_context.renderer = TextureTestRenderer.new do |cmd|
      all_cmds << cmd
    end

    ctx.dsl do
      window("Hello Nuklear", left: 10, top: 10, width: 180, height: 250,
                              border: true, no_scrollbar: true, movable: true) do
      end
    end

    ctx.tick

    all_cmds.each do |cmd|
      if cmd[:texture_handle] > 0
        # compare in hex for legibility
        assert_equal EXPECTED.to_s(16), cmd[:texture_handle].to_s(16)
      end
    end
  end
end
