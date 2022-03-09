module Nuklear
  module UI
    class Progress < Base
      attr_accessor :current, :min, :max
      attr_writer :modifiable

      def initialize(enabled: true, min: 1, max: 100, current: 1, modifiable: true)
        super enabled: enabled
        @min = min
        @max = max
        @current = current
        @modifiable = modifiable
        on(:changed) { yield @current } if block_given?
      end

      def to_command
        [:ui_progress, current, max, modifiable?]
      end

      def modifiable?
        @modifiable
      end

      def result(x, context)
        x = min if x < min
        return unless x != @current
        @current = x
        trigger :changed
        @last_event = nil
      end
    end
  end
end
