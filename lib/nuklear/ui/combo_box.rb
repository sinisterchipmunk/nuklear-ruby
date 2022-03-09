module Nuklear
  module UI
    class ComboBox < Nuklear::UI::Base
      include Nuklear::UI::Container

      attr_accessor :text, :image, :color, :symbol, :width, :height

      def initialize(text: nil, image: nil, color: nil, symbol: nil, width:, height:, **options)
        super(**options)
        self.text  = text
        self.image = image
        self.color = color
        self.symbol = symbol
        self.width = width
        self.height = height
      end

      def red; color.red; end
      def red=(a); color.red = a; end
      def green; color.green; end
      def green=(a); color.green = a; end
      def blue; color.blue; end
      def blue=(a); color.blue = a; end
      def alpha; color.alpha; end
      def alpha=(a); color.alpha = a; end
      def hue; color.hue; end
      def hue=(a); color.hue = a; end
      def saturation; color.saturation; end
      def saturation=(a); color.saturation = a; end
      def value; color.value; end
      def value=(a); color.value = a; end

      def to_command
        [ :ui_combobox, text, color, symbol, image, width, height ]
      end

      def result(expanded, context)
        run_commands(context) if expanded
      end
    end
  end
end
