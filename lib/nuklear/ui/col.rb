module Nuklear
  module UI
    class Col < Base
      include Nuklear::UI::Container
      attr_accessor :width

      def initialize(width:, **options)
        super(**options)
        self.width = width
      end

      def to_command
        [:ui_layout_row_push, width]
      end

      def result(_, context)
        run_commands(context)
      end
    end
  end
end
