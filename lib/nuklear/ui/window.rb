module Nuklear
  module UI
    class Window < Nuklear::UI::Base
      include Nuklear::UI::Container
      
      attr_accessor :title, :left, :top, :width, :height, :flags
      attr_writer :id
      attr_reader :content_area

      def initialize(title, id: object_id.to_s, left:, top:, width:, height:, flags: 0,
                     border: nil, movable: nil, scalable: nil, closable: nil,
                     minimizable: nil, no_scrollbar: nil, titlebar: nil,
                     scroll_auto_hide: nil, background: nil, scale_left: nil,
                     no_input: nil, enabled: true)
        super enabled: enabled

        @title = title
        @id = id
        @left = left
        @top = top
        @width = width
        @height = height
        @flags = Nuklear.parse_flags :window, flags
        @hiding = false
        @last_state = nil
        @content_area = Struct.new(:left, :top, :width, :height).new(left, top, width, height)

        self.border           = border           unless border.nil?
        self.movable          = movable          unless movable.nil?
        self.scalable         = scalable         unless scalable.nil?
        self.closable         = closable         unless closable.nil?
        self.minimizable      = minimizable      unless minimizable.nil?
        self.no_scrollbar     = no_scrollbar     unless no_scrollbar.nil?
        self.titlebar         = titlebar         unless titlebar.nil?
        self.scroll_auto_hide = scroll_auto_hide unless scroll_auto_hide.nil?
        self.background       = background       unless background.nil?
        self.scale_left       = scale_left       unless scale_left.nil?
        self.no_input         = no_input         unless no_input.nil?
      end

      def border?
        check_flag Nuklear::NK_WINDOW_BORDER
      end

      def movable?
        check_flag Nuklear::NK_WINDOW_MOVABLE
      end

      def scalable?
        check_flag Nuklear::NK_WINDOW_SCALABLE
      end

      def closable?
        check_flag Nuklear::NK_WINDOW_CLOSABLE
      end

      def minimizable?
        check_flag Nuklear::NK_WINDOW_MINIMIZABLE
      end

      def no_scrollbar?
        check_flag Nuklear::NK_WINDOW_NO_SCROLLBAR
      end

      def titlebar?
        check_flag Nuklear::NK_WINDOW_TITLE
      end

      def scroll_auto_hide?
        check_flag Nuklear::NK_WINDOW_SCROLL_AUTO_HIDE
      end

      def background?
        check_flag Nuklear::NK_WINDOW_BACKGROUND
      end

      def scale_left?
        check_flag Nuklear::NK_WINDOW_SCALE_LEFT
      end

      def no_input?
        check_flag Nuklear::NK_WINDOW_NO_INPUT
      end

      def border=(a)
        set_flag Nuklear::NK_WINDOW_BORDER, a
      end

      def movable=(a)
        set_flag Nuklear::NK_WINDOW_MOVABLE, a
      end

      def scalable=(a)
        set_flag Nuklear::NK_WINDOW_SCALABLE, a
      end

      def closable=(a)
        set_flag Nuklear::NK_WINDOW_CLOSABLE, a
      end

      def minimizable=(a)
        set_flag Nuklear::NK_WINDOW_MINIMIZABLE, a
      end

      def no_scrollbar=(a)
        set_flag Nuklear::NK_WINDOW_NO_SCROLLBAR, a
      end

      def titlebar=(a)
        set_flag Nuklear::NK_WINDOW_TITLE, a
      end

      def scroll_auto_hide=(a)
        set_flag Nuklear::NK_WINDOW_SCROLL_AUTO_HIDE, a
      end

      def background=(a)
        set_flag Nuklear::NK_WINDOW_BACKGROUND, a
      end

      def scale_left=(a)
        set_flag Nuklear::NK_WINDOW_SCALE_LEFT, a
      end

      def no_input=(a)
        set_flag Nuklear::NK_WINDOW_NO_INPUT, a
      end

      def set_flag(flag, enabled)
        @flags = enabled ? @flags | flag : @flags & ~flag
      end

      def check_flag(flag)
        @flags & flag > 0
      end

      def id
        @id || title
      end

      def enabled=(b)
        if @enabled && !b
          @hiding = true
        else
          super b
        end
      end

      def close
        self.enabled = false
      end

      def result(new_state, context)
        if new_state != @last_state
          @left, @top, @width, @height = *new_state[0...4]
          @content_area.left, @content_area.top, @content_area.width, @content_area.height = *new_state[4..-1]
          trigger(:resized)
          @last_event = nil
          @last_state = new_state
        end
        run_commands(context)
      end

      def to_command
        if @hiding
          @hiding = false
          @enabled = false
          [ :ui_window_close, id ]
        else
          [ :ui_window, id, title, flags, left, top, width, height ]
        end
      end
    end
  end
end
