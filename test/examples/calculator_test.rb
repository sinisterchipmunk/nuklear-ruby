require 'test_helper'
require_relative '../../examples/calculator'

class CalculatorTest < Minitest::Test
  include Nuklear::TestCase

  def setup
    super
    @calc = Calculator.new
    @nuklear = @calc.ctx
  end

  def tick
    @calc.tick
  end

  def teardown
    super
    @calc.close
  end

  def test_addition
    click_button '5'
    click_button '+'
    click_button '1'
    click_button '0'
    click_button '='
    assert_text '15.00'
  end

  def test_arithmetic
    click_button '5'
    click_button '*'
    click_button '3'
    assert_text '3'
    click_button '0'
    assert_text '30'
    click_button '='
    assert_text '150'

    click_button '/'
    click_button '2'
    assert_text '2'
    click_button '='
    assert_text '75'

    click_button '+'
    click_button '1'
    assert_text '1'
    click_button '-'
    click_button '2'
    assert_text '2'
    click_button '='
    assert_text '74'
  end
end
