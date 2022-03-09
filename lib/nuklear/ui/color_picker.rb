module Nuklear
  module UI
    class ColorPicker < Base
      attr_accessor :color
      attr_reader :type

      def initialize(color, type: :rgba, **options)
        super(**options)
        self.color = color
        self.type = type
      end

      def type=(t)
        case t
        when :rgb, :rgba then @type = t
        else raise ArgumentError, "invalid color picker type, must be :rgb or :rgba"
        end
      end

      def to_command
        [:ui_color_picker, color, type]
      end

      def result(color, context)
        @color = color
        trigger :color_changed
        @last_event = nil
      end
    end
  end
end
