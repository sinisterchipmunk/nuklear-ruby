require "test_helper"

class ContextTest < Minitest::Test
  # Returns true if there is an active or focused UI element, false otherwise.
  # Useful for determining whether to dispatch input events into Nuklear or
  # to handle them elsewhere.
  def test_nk_item_is_any_active
    refute context.any_item_active?
    w = Nuklear::UI::Window.new("Test window", left: 10, top: 10, width: 100, height: 100)
    w.layout_row_dynamic 100, 1
    e = Nuklear::UI::EditString.new
    w << e
    context << w
    context.tick

    refute context.any_item_active?

    context.trigger :motion, 92, 75, 0, 0
    context.tick
    assert context.any_item_active?

    context.trigger :motion, 200, 75, 0, 0
    context.tick
    refute context.any_item_active?

    context.trigger :motion, 75, 75, 0, 0
    context.trigger :button, :left, 75, 75, true
    context.tick
    context.trigger :button, :left, 75, 75, false
    context.trigger :glyph,  'a'
    assert context.any_item_active?
    assert context.any_editor_active?

    # we clicked on the text editor so at this point it should be expecting
    # text input, even if mouse moves away.
    context.trigger :motion, 200, 75, 0, 0
    context.tick
    assert context.any_item_active?
  end
end
