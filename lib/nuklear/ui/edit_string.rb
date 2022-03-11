module Nuklear
  module UI
    class EditString < Nuklear::UI::Base
      attr_accessor :flags, :max_length, :filter, :text
      attr_reader :text_was

      def initialize(text: "", flags: :simple, max_length: 255, filter: nil,
                     enabled: true, &on_change)
        super enabled: enabled
        @text         = text
        @flags        = Nuklear.parse_flags :edit, flags
        @max_length   = max_length
        @filter       = filter
        @demand_focus = false
        @text_was = text.dup
        on(:change, &on_change) if block_given?
      end

      def demand_focus?
        @demand_focus
      end

      def focus
        @demand_focus = true
      end

      def to_commands
        if demand_focus?
          @demand_focus = false
          [
            [:ui_edit_focus, flags],
            self,
          ]
        else
          [self]
        end
      end

      def to_command
        [:ui_edit_string, flags, text, max_length, filter]
      end

      def result(result, context)
        trigger(:activated)   if (result & Nuklear::NK_EDIT_ACTIVATED)   > 0
        trigger(:deactivated) if (result & Nuklear::NK_EDIT_DEACTIVATED) > 0
        trigger(:committed)   if (result & Nuklear::NK_EDIT_COMMITED)    > 0
        trigger(:active)      if (result & Nuklear::NK_EDIT_ACTIVE)      > 0
        trigger(:inactive)    if (result & Nuklear::NK_EDIT_INACTIVE)    > 0
        if @text != @text_was
          trigger :change
          @text_was.clear
          @text_was.concat @text
        end
      end
    end
  end
end
