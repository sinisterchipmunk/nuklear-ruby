require 'sdl2'
SDL2.init(SDL2::INIT_EVERYTHING)

module SDL2::Window::Flags
  ALLOW_HIGHDPI = 0x00002000 unless defined?(SDL2::Window::Flags::ALLOW_HIGHDPI)
end
