module Nuklear
  module UI
    module Enableable
      def enabled=(a)
        @enabled = a
      end

      def disabled=(a)
        self.enabled = !a
      end

      def enable
        self.enabled = true
      end

      def disable
        self.enabled = false
      end

      def enabled?
        @enabled.nil? ? true : @enabled
      end

      def disabled?
        !enabled?
      end
    end
  end
end
