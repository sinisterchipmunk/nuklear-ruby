module Nuklear
  module UI
    class Base
      include Nuklear::UI::Events
      include Nuklear::UI::Enableable

      def initialize(enabled: true)
        self.enabled = enabled
      end

      def dsl(&block)
        require 'nuklear/dsl'
        Nuklear::DSL.new(self, &block)
      end

      class << self
        attr_writer :dsl_method_name

        def dsl_method_name
          # Nukelar::UI::ColorPicker => 'color_picker'
          @dsl_method_name ||= name.split('::').last.gsub(/([A-Z])/) { |a| "_#{a.downcase}" }.sub(/^_/, '')
        end

        def descendants
          @descendants ||= []
        end

        def inherited(child)
          descendants << child
        end
      end
    end
  end
end
