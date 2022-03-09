module Nuklear
  module UI
    class Checkbox < Nuklear::UI::Base
      attr_accessor :text

      def initialize(text, checked: false, enabled: true, &block)
        super enabled: enabled
        @text = text
        @checked = checked
        on(true) { @checked = true }
        on(false) { @checked = false }
        if block_given?
          on true,  &block
          on false, &block
        end
      end

      def checked?
        @checked
      end

      def checked=(checked)
        @checked = checked
        @last_event = checked
      end

      alias selected? checked?
      alias selected= checked=

      def to_command
        [ :ui_checkbox, text, checked? ]
      end

      def result(event, context)
        trigger event
      end
    end
  end
end
