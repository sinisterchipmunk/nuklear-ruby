require "test_helper"

class BufferTest < Minitest::Test
  def test_fixed_size_buffer_and_fiddle_pointers
    buffer = Nuklear::Buffer.new 10
    assert_equal 10, buffer.size

    ptr = buffer.to_ptr
    assert_equal 10, ptr.size
  end
end
