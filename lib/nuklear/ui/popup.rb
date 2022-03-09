require 'nuklear/ui/window'

module Nuklear
  module UI
    class Popup < Nuklear::UI::Window
      attr_accessor :type

      def initialize(title, type: :static, enabled: false, **options)
        super title, enabled: enabled, **options
        self.type = type
        on(:visible) { self.enabled = true }
        on(:hidden) { self.enabled = false }
      end

      def to_command
        [:ui_popup,
          type == :dynamic ? NK_POPUP_DYNAMIC : NK_POPUP_STATIC,
          title,
          flags,
          left,
          top,
          width,
          height
        ]
      end

      def visible?
        @enabled
      end

      def result(visibility, context)
        trigger(visibility)
        run_commands(context) if visible?
      end
    end
  end
end
