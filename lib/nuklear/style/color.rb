module Nuklear
  module Style
    class Color
      # defined in C
      # attr_accessor :red, :green, :blue, :alpha
      # attr_accessor :hue, :saturation, :value

      def initialize(r, g, b, a = 1)
        self.red   = r
        self.green = g
        self.blue  = b
        self.alpha = a
      end

      def self.from_bytes(r, g, b, a = 255)
        new(r / 255.0, g / 255.0, b / 255.0, a / 255.0)
      end

      def inspect
        "#<#{self.class.name} red=#{red} green=#{green} blue=#{blue} alpha=#{alpha}>"
      end
    end
  end
end
