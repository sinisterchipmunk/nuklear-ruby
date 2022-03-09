module Nuklear
  module UI
    class Button < Nuklear::UI::Base
      include Nuklear::UI::TextAlign
      
      attr_accessor :text

      # A hash which may contain the following options:
      #    :normal          => a style item, see the classes contained in Nuklear::Style
      #    :hover           => a style item, see the classes contained in Nuklear::Style
      #    :active          => a style item, see the classes contained in Nuklear::Style
      #    :border_color    => an instance of Nuklear::Style::Color
      #    :text_background => an instance of Nuklear::Style::Color
      #    :text_normal     => an instance of Nuklear::Style::Color
      #    :text_hover      => an instance of Nuklear::Style::Color
      #    :text_active     => an instance of Nuklear::Style::Color
      #    :text_alignment  => a set of alignment flags
      #    :border          => a float
      #    :rounding        => a float
      #    :padding         => a vec2 of floats
      #    :image_padding   => a vec2 of floats
      #    :touch_padding   => a vec2 of floats
      attr_accessor :style

      # Whether the button click event will repeat while the button is pressed
      attr_writer :repeat

      attr_accessor :color

      # One of: nil, :x, :underscore, :circle_solid, :circle_outline,
      # :rect_solid, :rect_outline, :triangle_up, :triangle_down,
      # :triangle_left, :triangle_right, :plus, :minus or :max
      attr_accessor :symbol

      attr_accessor :image

      # nil, or a set of alignment flags
      attr_accessor :align

      def initialize(text = nil, style: nil, repeat: false, color: nil, symbol: nil, image: nil, align: nil, **options, &block)
        super(**options)
        @text = text
        @style = style
        @repeat = repeat
        @color = color
        @symbol = symbol
        @image = image
        @align = align
        on(:clicked, &block) if block_given?
      end

      def repeat?
        @repeat
      end

      def to_command
        [
          :ui_button, {
            repeat: repeat?,
            title: text,
            color: color,
            symbol: symbol,
            image: image,
            style: style,
            alignment: align_as_flags(align)
          }
        ]
      end

      def result(result, context)
        if result == 1 then trigger(:clicked)
        else trigger(:released)
        end
      end
    end
  end
end
