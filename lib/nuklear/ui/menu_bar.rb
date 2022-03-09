module Nuklear
  module UI
    class MenuBar < Nuklear::UI::Base
      include Nuklear::UI::Container

      def initialize(enabled: true)
        super enabled: enabled
      end

      def to_command
        [:ui_menubar]
      end

      def result(visible, context)
        run_commands(context) if visible
      end
    end
  end
end
