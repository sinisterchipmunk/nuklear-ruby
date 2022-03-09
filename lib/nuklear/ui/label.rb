module Nuklear
  module UI
    class Label < Nuklear::UI::Base
      include Nuklear::UI::TextAlign
      
      attr_accessor :text, :color, :wrap, :align

      def initialize(text, color: nil, wrap: false, align: :left, **options)
        super(**options)
        self.text  = text
        self.color = color
        self.wrap  = wrap
        self.align = align
      end

      def to_command
        [:ui_label, text, color, wrap, align_as_flags(align)]
      end
    end
  end
end
