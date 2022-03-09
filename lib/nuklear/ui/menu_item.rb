module Nuklear
  module UI
    class MenuItem < Nuklear::UI::Base
      include Nuklear::UI::Container
      include Nuklear::UI::TextAlign

      attr_accessor :title, :image, :symbol, :align

      def initialize(enabled: true, title: nil, image: nil, symbol: nil, align: :left, &block)
        super enabled: enabled
        @title = title
        @image = image
        @symbol = symbol
        @align = align
        on(:clicked, &block) if block_given?
      end

      def to_command
        [:ui_menu_item, {
            title: title,
            image: image,
            symbol: symbol,
            align: align_as_flags(align)
          }
        ]
      end

      def result(clicked, context)
        trigger(:clicked)
        @last_event = nil # trigger event each time
      end
    end
  end
end
