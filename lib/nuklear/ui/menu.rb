module Nuklear
  module UI
    class Menu < Nuklear::UI::Base
      include Nuklear::UI::Container
      include Nuklear::UI::TextAlign

      attr_accessor :title, :image, :symbol, :align, :width, :height

      def initialize(enabled: true, title: nil, image: nil, symbol: nil,
                     align: :left, width:, height:)
        super enabled: enabled
        @title = title
        @image = image
        @symbol = symbol
        @align = align
        @width = width
        @height = height
      end

      def to_command
        [:ui_menu, {
            id: object_id,
            title: title,
            image: image,
            symbol: symbol,
            align: align_as_flags(align),
            width: width,
            height: height
          }
        ]
      end

      def visible?
        @visible
      end

      def result(visible, context)
        @visible = visible
        run_commands(context) if visible
      end
    end
  end
end
