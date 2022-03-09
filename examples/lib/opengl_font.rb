# This class facilitates rendering text with Nuklear and presenting it using
# OpenGL. It is part of the nuklear-ruby examples.
module Examples
  class OpenGLFont
    attr_reader :path

    def initialize(path, size = 12)
      @path = path
      @size = size
      @texture_id = nil
      @nuklear_handle = nil
    end

    def texture_id
      @texture_id ||= begin
        tex_names_buf = ' ' * 8
        glGenTextures(1, tex_names_buf)
        tex_names_buf.unpack('L1')[0]
      end
    end

    def nuklear_handle
      @nuklear_handle ||= Nuklear::Font.new(path, @size) do |w, h, data|
        glBindTexture(GL_TEXTURE_2D, texture_id)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        texture_id
      end
    end
  end
end
