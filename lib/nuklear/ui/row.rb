module Nuklear
  module UI
    class Row < Base
      include Nuklear::UI::Container
      attr_accessor :height, :type, :width

      def initialize(height:, width: nil, type: nil, **options)
        super(**options)
        self.type = type
        self.height = height
        self.width = width
        raise ArgumentError, ":width is required if type is :static" if type == :static && width.nil?
      end

      def to_command
        case type
        when :dynamic then [:ui_layout_row_dynamic, height, commands.size]
        when :static  then [:ui_layout_row_static, height, width, commands.size]
        else [:ui_layout_row_begin, type || :static, height, commands.size]
        end
      end

      def result(_, context)
        run_commands(context)
      end
    end
  end
end
