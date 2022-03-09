/* Heavily adapted from https://github.com/vurtun/nuklear/blob/master/demo/sdl_opengl2/nuklear_sdl_gl2.h */

#define NK_IMPLEMENTATION
#include "../nuklear/nuklear.h"
#include "glad.h"
#include <ruby.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

VALUE mNuklear = Qnil;
VALUE cNuklearRenderer = Qnil;
VALUE cNuklearRendererOpenGL2 = Qnil;
VALUE cNuklearBuffer = Qnil;

struct vertex {
  float position[2];
  float uv[2];
  nk_byte col[4];
};

static enum nk_draw_vertex_layout_attribute rb2nk_attribute(VALUE v) {
  ID id = SYM2ID(v);
  if (id == rb_intern("position"))             return NK_VERTEX_POSITION;
  else if (id == rb_intern("color"))           return NK_VERTEX_COLOR;
  else if (id == rb_intern("texcoord"))        return NK_VERTEX_TEXCOORD;
  else if (id == rb_intern("attribute_count")) return NK_VERTEX_ATTRIBUTE_COUNT;
  else rb_raise(rb_eArgError, "Invalid vertex layout attribute name");
}

static enum nk_draw_vertex_layout_format rb2nk_format(VALUE v) {
  ID id = SYM2ID(v);
  if (id == rb_intern("schar"))                    return NK_FORMAT_SCHAR;
  else if (id == rb_intern("sshort"))              return NK_FORMAT_SSHORT;
  else if (id == rb_intern("sint"))                return NK_FORMAT_SINT;
  else if (id == rb_intern("uchar"))               return NK_FORMAT_UCHAR;
  else if (id == rb_intern("ushort"))              return NK_FORMAT_USHORT;
  else if (id == rb_intern("uint"))                return NK_FORMAT_UINT;
  else if (id == rb_intern("float"))               return NK_FORMAT_FLOAT;
  else if (id == rb_intern("double"))              return NK_FORMAT_DOUBLE;
  else if (id == rb_intern("r8g8b8"))              return NK_FORMAT_R8G8B8;
  else if (id == rb_intern("r16g15b16"))           return NK_FORMAT_R16G15B16;
  else if (id == rb_intern("r32g32b32"))           return NK_FORMAT_R32G32B32;
  else if (id == rb_intern("r8g8b8a8"))            return NK_FORMAT_R8G8B8A8;
  else if (id == rb_intern("b8g8r8a8"))            return NK_FORMAT_B8G8R8A8;
  else if (id == rb_intern("r16g15b16a16"))        return NK_FORMAT_R16G15B16A16;
  else if (id == rb_intern("r32g32b32a32"))        return NK_FORMAT_R32G32B32A32;
  else if (id == rb_intern("r32g32b32a32_float"))  return NK_FORMAT_R32G32B32A32_FLOAT;
  else if (id == rb_intern("r32g32b32a32_double")) return NK_FORMAT_R32G32B32A32_DOUBLE;
  else if (id == rb_intern("rgb32"))               return NK_FORMAT_RGB32;
  else if (id == rb_intern("rgba32"))              return NK_FORMAT_RGBA32;
  else rb_raise(rb_eArgError, "Invalid vertex layout format name");
}

VALUE renderer_initialize(VALUE self, VALUE context) {
  if (!gladLoadGL())
    rb_raise(rb_eStandardError, "Failed to init GLAD");
  // printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

  rb_call_super(1, &context);

  return self;
}

VALUE renderer_render_gl(VALUE self) {
  VALUE window_size = rb_funcall(self, rb_intern("window_size"), 0);
  VALUE drawable_size = rb_funcall(self, rb_intern("drawable_size"), 0);
  VALUE window_width = rb_ary_entry(window_size, 0);
  VALUE window_height = rb_ary_entry(window_size, 1);
  VALUE drawable_width = rb_ary_entry(drawable_size, 0);
  VALUE drawable_height = rb_ary_entry(drawable_size, 1);
  int width = FIX2INT(window_width),
      height = FIX2INT(window_height);
  int display_width = FIX2INT(drawable_width),
      display_height = FIX2INT(drawable_height);

  glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT|GL_TRANSFORM_BIT);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* setup viewport/project */
  glViewport(0,0,(GLsizei)display_width,(GLsizei)display_height);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  rb_call_super(0, NULL);
  
  /* default OpenGL state */
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  return self;
}

VALUE renderer_nk_convert_gl(VALUE self) {
  VALUE result = rb_call_super(0, NULL);
  VALUE rconfig = rb_funcall(self, rb_intern("convert_config"), 0);
  GLsizei vs = (GLsizei) FIX2INT(rb_hash_aref(rconfig, ID2SYM(rb_intern("vertex_size"))));

  size_t vp, vt, vc; // vertex struct offsets
  VALUE rvertex_layout = rb_hash_aref(rconfig, ID2SYM(rb_intern("vertex_layout")));
  for (int i = 0; i < RARRAY_LEN(rvertex_layout); i++) {
    VALUE rlayout = RARRAY_AREF(rvertex_layout, i);
    int offset = FIX2INT(RARRAY_AREF(rlayout, 2));
    enum nk_draw_vertex_layout_attribute attr = rb2nk_attribute(RARRAY_AREF(rlayout, 0));
    switch(attr) {
      case NK_VERTEX_POSITION: vp = (size_t) offset; break;
      case NK_VERTEX_COLOR:    vc = (size_t) offset; break;
      case NK_VERTEX_TEXCOORD: vt = (size_t) offset; break;
      default: break;
    }
  }

  /* setup vertex buffer pointer */
  struct nk_buffer *vertex_buffer = NULL;
  Data_Get_Struct(rb_funcall(self, rb_intern("vertices"), 0), struct nk_buffer, vertex_buffer);
  const void *vertices = nk_buffer_memory_const(vertex_buffer);
  glVertexPointer(2, GL_FLOAT, vs, (const void*)((const nk_byte*)vertices + vp));
  glTexCoordPointer(2, GL_FLOAT, vs, (const void*)((const nk_byte*)vertices + vt));
  glColorPointer(4, GL_UNSIGNED_BYTE, vs, (const void*)((const nk_byte*)vertices + vc));

  return result;
}

VALUE renderer_draw_gl(VALUE self, VALUE cmd) {
  VALUE window_size = rb_funcall(self, rb_intern("window_size"), 0);
  VALUE drawable_size = rb_funcall(self, rb_intern("drawable_size"), 0);
  VALUE window_width = rb_ary_entry(window_size, 0);
  VALUE window_height = rb_ary_entry(window_size, 1);
  VALUE drawable_width = rb_ary_entry(drawable_size, 0);
  VALUE drawable_height = rb_ary_entry(drawable_size, 1);
  int width = FIX2INT(window_width),
      height = FIX2INT(window_height);
  int display_width = FIX2INT(drawable_width),
      display_height = FIX2INT(drawable_height);
  struct nk_vec2 scale;
  scale.x = (float)display_width/(float)width;
  scale.y = (float)display_height/(float)height;

  VALUE clip_rect = rb_hash_aref(cmd, ID2SYM(rb_intern("clip_rect")));
  float x = (float) NUM2DBL(RARRAY_AREF(clip_rect, 0));
  float y = (float) NUM2DBL(RARRAY_AREF(clip_rect, 1));
  float w = (float) NUM2DBL(RARRAY_AREF(clip_rect, 2));
  float h = (float) NUM2DBL(RARRAY_AREF(clip_rect, 3));
  printf("%d\n", NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("texture_handle")))));
  glBindTexture(GL_TEXTURE_2D, (GLuint) NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("texture_handle")))));
  glScissor((GLint)(x * scale.x),
            (GLint)((height - (GLint)(y + h)) * scale.y),
            (GLint)(w * scale.x),
            (GLint)(h * scale.y));
  unsigned int num_elements = NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("element_count"))));

  struct nk_buffer *element_buffer = NULL;
  Data_Get_Struct(rb_funcall(self, rb_intern("vertex_indices"), 0), struct nk_buffer, element_buffer);
  const nk_draw_index *offset = (const nk_draw_index *) nk_buffer_memory_const(element_buffer);
  offset += NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("offset"))));
  glDrawElements(GL_TRIANGLES, (GLsizei)num_elements, GL_UNSIGNED_SHORT, (void *) offset);
  return self;
}

void Init_nuklear_renderer_opengl2(void) {
  mNuklear = rb_define_module("Nuklear");
  cNuklearRenderer = rb_define_class_under(mNuklear, "Renderer", rb_cObject);
  cNuklearRendererOpenGL2 = rb_define_class_under(cNuklearRenderer, "OpenGL2", cNuklearRenderer);
  cNuklearBuffer = rb_const_get(mNuklear, rb_intern("Buffer"));

  rb_define_method(cNuklearRendererOpenGL2, "initialize", renderer_initialize, 1);
  rb_define_method(cNuklearRendererOpenGL2, "nk_convert", renderer_nk_convert_gl, 0);
  rb_define_method(cNuklearRendererOpenGL2, "draw",       renderer_draw_gl, 1);
  rb_define_method(cNuklearRendererOpenGL2, "render",     renderer_render_gl, 0);
}
