require 'sdl2'

module Examples
  module SDL2Input
    def process_sdl2_events(nuklear_ctx, escape_keys: [SDL2::Key::ESCAPE])
      while event = ::SDL2::Event.poll
        case event
        # TODO touch events
        # when SDL2::Event::FingerMotion
        # when SDL2::Event::FingerDown
        # when SDL2::Event::FingerUp
        when ::SDL2::Event::TextInput   then nuklear_ctx.trigger(:glyph,  event.text)
        when ::SDL2::Event::MouseMotion then nuklear_ctx.trigger(:motion, event.x, event.y, event.xrel, event.yrel)
        when ::SDL2::Event::MouseWheel  then nuklear_ctx.trigger(:scroll, event.x, event.y)
        when ::SDL2::Event::MouseButtonDown, ::SDL2::Event::MouseButtonUp
          is_down = event.kind_of?(::SDL2::Event::MouseButtonDown)
          if event.button == 1 and event.clicks > 1
            nuklear_ctx.trigger(:button, :double, event.x, event.y, is_down)
          else
            case event.button
            # SDL_BUTTON_LEFT seems to be not defined
            when 1 then nuklear_ctx.trigger(:button, :left,   event.x, event.y, is_down)
            when 2 then nuklear_ctx.trigger(:button, :middle, event.x, event.y, is_down)
            when 3 then nuklear_ctx.trigger(:button, :right,  event.x, event.y, is_down)
            else yield event if block_given?
            end
          end
        when ::SDL2::Event::Window
          case event.event
          when ::SDL2::Event::Window::FOCUS_GAINED then @focused = true
          when ::SDL2::Event::Window::FOCUS_LOST   then @focused = false
          end
        when ::SDL2::Event::KeyDown, ::SDL2::Event::KeyUp
          is_down = event.kind_of?(::SDL2::Event::KeyDown)
          case event.sym
          when ::SDL2::Key::LSHIFT, ::SDL2::Key::RSHIFT then nuklear_ctx.trigger(:key, :shift,           is_down)
          when ::SDL2::Key::DELETE                      then nuklear_ctx.trigger(:key, :del,             is_down)
          when ::SDL2::Key::RETURN                      then nuklear_ctx.trigger(:key, :enter,           is_down)
          when ::SDL2::Key::TAB                         then nuklear_ctx.trigger(:key, :tab,             is_down)
          when ::SDL2::Key::BACKSPACE                   then nuklear_ctx.trigger(:key, :backspace,       is_down)
          when ::SDL2::Key::HOME                        then nuklear_ctx.trigger(:key, :text_start,      is_down)
                                                             nuklear_ctx.trigger(:key, :scroll_start,    is_down)
          when ::SDL2::Key::END                         then nuklear_ctx.trigger(:key, :text_end,        is_down)
                                                             nuklear_ctx.trigger(:key, :scroll_end,      is_down)
          when ::SDL2::Key::PAGEDOWN                    then nuklear_ctx.trigger(:key, :scroll_down,     is_down)
          when ::SDL2::Key::PAGEUP                      then nuklear_ctx.trigger(:key, :scroll_up,       is_down)
          when ::SDL2::Key::Z                           then nuklear_ctx.trigger(:key, :text_undo,       is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::R                           then nuklear_ctx.trigger(:key, :text_redo,       is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::C                           then nuklear_ctx.trigger(:key, :copy,            is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::V                           then nuklear_ctx.trigger(:key, :paste,           is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::X                           then nuklear_ctx.trigger(:key, :cut,             is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::B                           then nuklear_ctx.trigger(:key, :text_line_start, is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::E                           then nuklear_ctx.trigger(:key, :text_line_end,   is_down && (event.mod & ::SDL2::Key::Mod::CTRL > 0))
          when ::SDL2::Key::UP                          then nuklear_ctx.trigger(:key, :up,              is_down)
          when ::SDL2::Key::DOWN                        then nuklear_ctx.trigger(:key, :down,            is_down)
          when ::SDL2::Key::LEFT
            if event.mod & ::SDL2::Key::Mod::CTRL > 0   then nuklear_ctx.trigger(:key, :text_word_left,  is_down)
            else nuklear_ctx.trigger(:key, :left, is_down)
            end
          when ::SDL2::Key::RIGHT
            if event.mod & ::SDL2::Key::Mod::CTRL > 0   then nuklear_ctx.trigger(:key, :text_word_right, is_down)
            else nuklear_ctx.trigger(:key, :right, is_down)
            end
          end

          yield event if block_given? && escape_keys.include?(event.sym)
        when SDL2::Event::Quit then exit
        when SDL2::Event::KeyDown
          case event.sym
          when SDL2::Key::C
            if (event.mod & SDL2::Key::Mod::CTRL) > 0
              exit
            end
          end
        else
          yield event if block_given?
        end
      end
    end
  end
end
