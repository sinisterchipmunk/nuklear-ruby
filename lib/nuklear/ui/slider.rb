module Nuklear
  module UI
    class Slider < Base
      attr_accessor :current, :min, :max, :step

      def initialize(enabled: true, min: 0.0, max: 1.0, current: 0.0, step: 0.001)
        super enabled: enabled
        @min = min
        @max = max
        @current = current
        @step = step
        on(:changed) { yield @current } if block_given?
      end

      def to_command
        [:ui_slider, current, min, max, step]
      end

      def result(x, context)
        @current = x
        trigger :changed
        @last_event = nil
      end
    end
  end
end
