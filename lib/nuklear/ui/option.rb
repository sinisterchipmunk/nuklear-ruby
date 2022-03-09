module Nuklear
  module UI
    class Option < Nuklear::UI::Checkbox
      def to_command
        [ :ui_option, text, checked? ]
      end

      # Override #result to prevent user deselecting this option by clicking
      # on it. Checkbox can be deselected, option cannot. Only choosing a
      # competing option can cause it to be deselected, and doing that does
      # not call #result. See Nuklear::UI::OptionGroup.
      def result(event, context)
        trigger event if event
      end
    end
  end
end
