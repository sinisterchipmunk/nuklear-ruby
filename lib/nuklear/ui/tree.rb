module Nuklear
  module UI
    class Tree < Nuklear::UI::Base
      include Nuklear::UI::Container

      attr_accessor :title
      attr_accessor :image

      def initialize(title, image: nil, type: :tab, state: :minimized, **options)
        super(**options)

        self.title = title
        self.image = image
        self.type  = type
        self.state = state

        on(Nuklear::NK_MINIMIZED, Nuklear::NK_MAXIMIZED) { |new_state| @state = new_state }
      end

      def type
        case type
        when Nuklear::NK_TREE_TAB then :tab
        when Nuklear::NK_TREE_NODE then :node
        end
      end

      def type=(type)
        case type
        when :tab then @type = Nuklear::NK_TREE_TAB
        when :node then @type = Nuklear::NK_TREE_NODE
        when Nuklear::NK_TREE_TAB, Nuklear::NK_TREE_NODE then @type = type
        else raise ArgumentError, "type must be one of NK_TREE_TAB, NK_TREE_NODE"
        end
      end

      def state
        case type
        when Nuklear::NK_MAXIMIZED then :maximized
        when Nuklear::NK_MINIMIZED then :minimized
        end
      end

      def state=(state)
        case state
        when :maximized then @state = Nuklear::NK_MAXIMIZED
        when :minimized then @state = Nuklear::NK_MINIMIZED
        when Nuklear::NK_MAXIMIZED, Nuklear::NK_MINIMIZED then @state = state
        else raise ArgumentError, "state must be one of NK_MAXIMIZED, NK_MINIMIZED"
        end
      end

      def to_command
        [ :ui_tree, @type, image, title, @state ]
      end

      def result(new_state, context)
        trigger(new_state)
        run_commands(context) if @state == Nuklear::NK_MAXIMIZED
      end
    end
  end
end
