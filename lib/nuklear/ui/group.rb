module Nuklear
  module UI
    class Group < Nuklear::UI::Base
      include Nuklear::UI::Container

      attr_accessor :title, :scroll_x, :scroll_y

      def initialize(title = object_id.to_s, **options)
        super(**options)
        @scroll_x = @scroll_y = 0
        self.title = title
      end

      def to_command
        [ :ui_group, title, flags, scroll_x, scroll_y ]
      end

      def flags
        0
      end

      def result(new_state, context)
        if new_state.kind_of?(Array)
          @scroll_x, @scroll_y = new_state
        else
          run_commands(context)
        end
      end
    end
  end
end
