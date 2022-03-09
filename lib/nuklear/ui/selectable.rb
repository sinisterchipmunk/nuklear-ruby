module Nuklear
  module UI
    class Selectable < Nuklear::UI::Checkbox
      include Nuklear::UI::TextAlign

      attr_accessor :image, :text, :align

      def initialize(text: nil, align: :left, image: nil, selected: false, **options, &block)
        super(**options, &block)
        self.selected = selected
        @text = text
        @image = image
        @align = align
      end

      def to_command
        [ :ui_selectable, image, text, selected?, align_as_flags(align) ]
      end
    end
  end
end
