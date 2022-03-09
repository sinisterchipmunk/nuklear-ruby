require "nuklear/version"
require "nuklear/nuklear"
require 'nuklear/context'
require 'nuklear/event_buffer'
require 'nuklear/ui'
require 'nuklear/renderer'
require 'nuklear/style'
require 'nuklear/dsl'

module Nuklear
  autoload :TestCase, 'nuklear/test_case'

  class Error < StandardError; end
  # Your code goes here...

  class << self
    def parse_flags(prefix, *flags)
      flags.flatten.reduce(0) do |set, flag|
        set | case flag
              when Symbol, String then Nuklear.const_get("NK_#{prefix.upcase}_#{flag.upcase}")
              else flag
              end
      end
    end
  end
end
