require "test_helper"

class BufferTest < Minitest::Test
  def test_fixed_size_buffer_and_fiddle_pointers
    buffer = Nuklear::Buffer.new 10
    assert_equal 10, buffer.size

    ptr = buffer.to_ptr
    assert_equal 10, ptr.size

    ptr = Fiddle::Pointer.malloc(15)
    buffer = Nuklear::Buffer.new ptr.size, ptr
    assert_equal 15, buffer.size
    assert_equal ptr, buffer.to_ptr
  end
end
