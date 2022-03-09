module Nuklear
  module UI
    class OptionGroup < Base
      include Nuklear::UI::Container

      def initialize(**options)
        super(**options)
        @options = []
      end

      def selection=(selected)
        @options.each { |option| option.selected = (option == selected) }
      end

      def <<(other)
        super other
        @options << other
        other.on(true) { self.selection = other }
      end
    end
  end
end
