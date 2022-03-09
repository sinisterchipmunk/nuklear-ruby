#include "nkrb.h"

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

VALUE nkrb_renderer_convert(VALUE self) {
  struct nk_context *context = NULL;
  struct nk_buffer *commands = NULL;
  struct nk_buffer *vertices = NULL;
  struct nk_buffer *indices  = NULL;
  Data_Get_Struct(rb_funcall(self, rb_intern("context"),  0),       struct nk_context, context);
  Data_Get_Struct(rb_funcall(self, rb_intern("commands"), 0),       struct nk_buffer,  commands);
  Data_Get_Struct(rb_funcall(self, rb_intern("vertices"), 0),       struct nk_buffer,  vertices);
  Data_Get_Struct(rb_funcall(self, rb_intern("vertex_indices"), 0), struct nk_buffer,  indices);
  VALUE rconfig = rb_funcall(self, rb_intern("convert_config"), 0);
  VALUE rvertex_layout = rb_hash_aref(rconfig, ID2SYM(rb_intern("vertex_layout")));
  struct nk_convert_config config;
  struct nk_draw_vertex_layout_element *vertex_layout = alloca((RARRAY_LEN(rvertex_layout) + 1) * sizeof(struct nk_draw_vertex_layout_element));
  for (int i = 0; i < RARRAY_LEN(rvertex_layout); i++) {
    VALUE rlayout = RARRAY_AREF(rvertex_layout, i);
    vertex_layout[i].attribute = rb2nk_attribute(RARRAY_AREF(rlayout, 0));
    vertex_layout[i].format    = rb2nk_format(RARRAY_AREF(rlayout, 1));
    vertex_layout[i].offset    = FIX2INT(RARRAY_AREF(rlayout, 2));
  }
  vertex_layout[RARRAY_LEN(rvertex_layout)].attribute = NK_VERTEX_LAYOUT_END;
  memset(&config, 0, sizeof(config));
  config.vertex_layout = vertex_layout;
  config.vertex_size = FIX2INT(rb_hash_aref(rconfig, ID2SYM(rb_intern("vertex_size"))));
  config.vertex_alignment = FIX2INT(rb_hash_aref(rconfig, ID2SYM(rb_intern("vertex_alignment"))));
  config.null.texture = (nk_handle) (void *) NUM2ULL(rb_hash_aref(rconfig, ID2SYM(rb_intern("null"))));
  config.circle_segment_count = NUM2UINT(rb_hash_aref(rconfig, ID2SYM(rb_intern("circle_segment_count"))));
  config.curve_segment_count = NUM2UINT(rb_hash_aref(rconfig, ID2SYM(rb_intern("curve_segment_count"))));
  config.arc_segment_count = NUM2UINT(rb_hash_aref(rconfig, ID2SYM(rb_intern("arc_segment_count"))));
  config.global_alpha = (float) NUM2DBL(rb_hash_aref(rconfig, ID2SYM(rb_intern("global_alpha"))));
  config.shape_AA = RTEST(rb_hash_aref(rconfig, ID2SYM(rb_intern("shape_aa")))) ? NK_ANTI_ALIASING_ON : NK_ANTI_ALIASING_OFF;
  config.line_AA = RTEST(rb_hash_aref(rconfig, ID2SYM(rb_intern("line_aa")))) ? NK_ANTI_ALIASING_ON : NK_ANTI_ALIASING_OFF;
  // all that for this
  nk_flags result = nk_convert(context, commands, vertices, indices, &config);
  switch(result) {
    case NK_CONVERT_SUCCESS: break; // OK
    case NK_CONVERT_INVALID_PARAM: rb_raise(rb_eRuntimeError, "Nuklear error: INVALID_PARAM");
    case NK_CONVERT_COMMAND_BUFFER_FULL: rb_raise(rb_eRuntimeError, "Nuklear error: COMMAND_BUFFER_FULL");
    case NK_CONVERT_VERTEX_BUFFER_FULL: rb_raise(rb_eRuntimeError, "Nuklear error: VERTEX_BUFFER_FULL");
    case NK_CONVERT_ELEMENT_BUFFER_FULL: rb_raise(rb_eRuntimeError, "Nuklear error: ELEMENT_BUFFER_FULL");
    default: rb_raise(rb_eRuntimeError, "Nuklear error: %d (unknown)", result);
  }
  return self;
}

VALUE nkrb_renderer_draw_foreach(VALUE self) {
  struct nk_context *context = NULL;
  struct nk_buffer *commands = NULL;
  Data_Get_Struct(rb_funcall(self, rb_intern("context"),  0), struct nk_context, context);
  Data_Get_Struct(rb_funcall(self, rb_intern("commands"), 0), struct nk_buffer,  commands);

  /* convert from command queue into draw list and draw to screen */
  const struct nk_draw_command *cmd;
  // const nk_draw_index *offset = NULL;
  /* iterate over and execute each draw command */
  int offset = 0;
  nk_draw_foreach(cmd, context, commands) {
    if (!cmd->elem_count) continue;
    VALUE rcmd = rb_hash_new();
    VALUE rclip_rect = rb_ary_new2(4);
    rb_ary_store(rclip_rect, 0, DBL2NUM(cmd->clip_rect.x));
    rb_ary_store(rclip_rect, 1, DBL2NUM(cmd->clip_rect.y));
    rb_ary_store(rclip_rect, 2, DBL2NUM(cmd->clip_rect.w));
    rb_ary_store(rclip_rect, 3, DBL2NUM(cmd->clip_rect.h));
    rb_hash_aset(rcmd, ID2SYM(rb_intern("element_count")),  INT2NUM(cmd->elem_count));
    rb_hash_aset(rcmd, ID2SYM(rb_intern("texture_handle")), ULL2NUM((unsigned long long) cmd->texture.ptr));
    rb_hash_aset(rcmd, ID2SYM(rb_intern("clip_rect")), rclip_rect);
    rb_hash_aset(rcmd, ID2SYM(rb_intern("offset")), INT2NUM(offset));
    rb_yield(rcmd);
    offset += cmd->elem_count;
  }

  return self;
}

void nkrb_renderer_init(void) {
  VALUE cRenderer = rb_define_class_under(mNuklear, "Renderer", rb_cObject);
  rb_define_method(cRenderer, "nk_convert",      nkrb_renderer_convert,      0);
  rb_define_method(cRenderer, "nk_draw_foreach", nkrb_renderer_draw_foreach, 0);
}
