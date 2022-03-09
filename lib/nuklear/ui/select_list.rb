module Nuklear
  module UI
    class SelectList < Nuklear::UI::Base
      include Nuklear::UI::Container

      attr_accessor :choices, :selection, :item_height, :width, :height

      def initialize(choices, selection: choices[0], item_height:, width:, height:, **options)
        super(**options)
        self.choices = choices
        self.selection = selection
        self.item_height = item_height
        self.width = width
        self.height = height
        on(:selection_changed) { yield self.selection } if block_given?
      end

      def to_command
        [ :ui_combo, choices.index(selection) || 0, choices, item_height, width, height ]
      end

      def result(choice_index, context)
        if @selection != choices[choice_index]
          @selection = choices[choice_index]
          trigger(:selection_changed)
          @last_event = nil
        end
      end
    end
  end
end
