require "test_helper"

class Nuklear::UI::EditStringTest < Minitest::Test
  # Returns true if there is an active or focused UI element, false otherwise.
  # Useful for determining whether to dispatch input events into Nuklear or
  # to handle them elsewhere.
  def test_nk_item_is_any_active
    refute context.any_item_active?
    w = Nuklear::UI::Window.new("Test window", left: 10, top: 10, width: 100, height: 100)
    w.layout_row_dynamic 100, 1
    changed_to = nil
    e = Nuklear::UI::EditString.new { changed_to = [e.text_was.dup, e.text.dup] }
    w << e
    context << w
    context.tick

    context.trigger :motion, 75, 75, 0, 0
    context.trigger :button, :left, 75, 75, true
    context.tick
    context.trigger :button, :left, 75, 75, false
    context.trigger :glyph,  'a'
    context.tick
    assert_equal 'a', e.text
    assert_equal ['', 'a'], changed_to # did event listener execute?
  end
end
