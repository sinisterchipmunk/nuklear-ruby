module Nuklear
  module UI
    module TextAlign
      def align_as_flags(align)
        case align
        when :left,   :west   then Nuklear::NK_TEXT_LEFT
        when :center, :middle then Nuklear::NK_TEXT_CENTERED
        when :right,  :east   then Nuklear::NK_TEXT_RIGHT
        else align
        end
      end
    end
  end
end
