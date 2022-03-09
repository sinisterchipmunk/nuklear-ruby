module Nuklear
  module UI
    class Property < Base
      attr_accessor :current, :min, :max, :step, :inc_per_pixel, :text

      def initialize(enabled: true, text: '', min: 1, max: 100, current: 1, step: 1, inc_per_pixel: 1)
        super enabled: enabled
        @text = text
        @min = min
        @max = max
        @current = current
        @step = step
        @inc_per_pixel = inc_per_pixel
        on(:changed) { yield @current } if block_given?
      end

      def to_command
        [:ui_property, text, min, current, max, step, inc_per_pixel]
      end

      def result(x, context)
        @current = x
        trigger :changed
        @last_event = nil
      end
    end
  end
end
