$LOAD_PATH.unshift File.expand_path("../lib", __dir__)
require "nuklear"

require "minitest/autorun"

class Minitest::Test
  def font_path
    File.expand_path('../examples/arial.ttf', __dir__)
  end

  def dummy_font
    @dummy_font ||= Nuklear::Font.new(font_path, 12) { 0 }
  end

  def context
    @context ||= Nuklear::Context.new(dummy_font).tap do |ctx|
      ctx.renderer = renderer
    end
  end

  def renderer
    @renderer ||= Nuklear::Renderer.new.tap do |renderer|
      renderer.window_size = [1024, 768]
      renderer.drawable_size = [1024, 768]
    end
  end
end
