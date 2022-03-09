module Nuklear
  module TestCase
    def nuklear
      @nuklear || raise("Nuklear::TestCase requires you to assign a Nuklear::Context to @nuklear during your test setup.")
    end

    def tick
      raise "Nuklear::TestCase requires you to override #tick to process window events and to call your Nuklear::Context#tick method."
    end

    def click_button(text)
      els = nuklear.find do |item|
        item.kind_of?(Nuklear::UI::Button) && item.respond_to?(:text) && item.text[text]
      end
      if els.count > 1
        raise "Ambiguous text: #{text} (found #{els.inspect})"
      elsif els.empty?
        raise "Cannot find an element with text: #{text.inspect}"
      else
        els.first.trigger(:clicked)
      end
      tick
    end

    def assert_text(txt)
      refute_empty nuklear.find { |el| el.respond_to?(:text) && el.text[txt] },
                   "Expected to find at least one element with text: #{txt.inspect}"
    end
  end
end
