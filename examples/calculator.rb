#!/usr/bin/env ruby
#
# This example is based loosely upon:
# https://github.com/Immediate-Mode-UI/Nuklear/blob/master/demo/calculator.c
#
# This demonstrates using the Nuklear DSL to build and maintain a simple but
# functional calculator. In order to show you only the most relevant
# information, much of the set-up code has been moved to /examples/lib/ and
# this file focuses mainly on the DSL. If you are more interested in the
# set-up code and event loop, you will find that the files in /examples/lib/
# are quite similar to what appears in /examples/hello_nuklear.rb.
#
require 'bundler/setup'
require 'nuklear'
require_relative 'lib/sdl2_init'
require_relative 'lib/sdl2_input'
require_relative 'lib/opengl_init'
require_relative 'lib/opengl_font'
require_relative 'lib/window'

class Calculator
  attr_reader :ctx, :window

  def initialize(hidden: false)
    @window = Examples::Window.new "Calculator Example", hidden: hidden
    font = Examples::OpenGLFont.new File.expand_path("arial.ttf", __dir__)
    @ctx = Nuklear::Context.new font.nuklear_handle
    @window.nuklear_context = @ctx

    numbers = '789456123'
    ops = '+-*/'
    operands = [0, 0]
    current = 0
    set = op = nil

    @ctx.dsl do
      window("Hello Nuklear", left: 10, top: 10, width: 180, height: 250,
                              border: true, no_scrollbar: true, movable: true) do
        layout_row_dynamic 35, 1
        label = edit_string text: "0.0", flags: :simple, filter: :float, max_length: 255

        layout_row_dynamic 35, 4
        update = proc { label.text = '%.2f' % operands[current] }
        solver = proc do
          # operands[0] = operands[0] + operands[1]
          operands[0] = operands[0].send(op, operands[1])
          label.text = '%.2f' % operands[0]
          current = 1
          operands[1] = 0
          set = true
        end
        16.times do |i|
          if i >= 12 && i < 15
            next if i > 12
            button("C") { operands = [0, 0]; current = 0; op = nil; update.call }
            button("0") { operands[current] *= 10; update.call }
            button("=") { solver.call; op = nil }
          elsif (i + 1) % 4 > 0
            n = numbers[(i/4)*3+i%4]
            button(n) do
              operands[current] = operands[current] * 10 + n.to_i
              set = nil
              update.call
            end
          else
            n = ops[i/4]
            button(n) do
              if !set
                if current != 1
                  current = 1
                  update.call
                else
                  solver.call
                end
              end
              op = ops[i/4]
              set = true
            end
          end
        end
      end
    end
  end

  def tick
    @window.frame do
      @ctx.tick
    end
  end

  def close
    @window.close
  end
end

if $0 == __FILE__
  calculator = Calculator.new
  while true
    # process window events and render any display changes
    calculator.tick
  end
end
