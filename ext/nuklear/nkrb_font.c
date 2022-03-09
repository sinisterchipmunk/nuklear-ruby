#include "nkrb.h"

typedef struct {
  struct { int width, height; const void *data; } img;
  struct nk_font_atlas atlas;
  struct nk_font *font;
  struct nk_draw_null_texture null;
} NkRbFont;

#define VALUE_TO_NK(value, varname)     \
  NkRbFont *varname = NULL;  \
  Data_Get_Struct(value, NkRbFont, varname);

VALUE cNuklearFont = Qnil;
VALUE cNuklearNullTexture = Qnil;

struct nk_user_font *nkrb_font_to_nk(VALUE value) {
  VALUE_TO_NK(value, nk);
  return &(nk->font->handle);
}

void nkrb_null_texture_free(struct nk_draw_null_texture *tex) {
  free(tex);
}

VALUE nkrb_font_get_null(VALUE value) {
  VALUE_TO_NK(value, nk);
  struct nk_draw_null_texture *tex = malloc(sizeof(struct nk_draw_null_texture));
  memcpy(tex, &(nk->null), sizeof(struct nk_draw_null_texture));
  return Data_Wrap_Struct(cNuklearNullTexture, NULL, nkrb_null_texture_free, tex);
}

void nkrb_font_free(NkRbFont *font) {
  nk_font_atlas_clear(&(font->atlas));
}

VALUE nkrb_font_width(VALUE self, VALUE rtext) {
  const char *text = StringValueCStr(rtext);
  struct nk_user_font *handle = nkrb_font_to_nk(self);
  float width = handle->width(handle->userdata, handle->height, text, (int) strlen(text));
  return DBL2NUM(width);
}

VALUE nkrb_font_height(VALUE self) {
  struct nk_user_font *handle = nkrb_font_to_nk(self);
  return DBL2NUM(handle->height);
}

VALUE nkrb_font_new(int argc, VALUE *argv, VALUE self) {
  if (argc < 2 || argc > 3)
    rb_raise(rb_eArgError, "Invalid number of arguments (%d for 2 or 3): pass path, size, and an optional arguments hash", argc);
  VALUE path = argv[0];
  VALUE size = argv[1];
  struct nk_font_config config;
  config = nk_font_config((float) NUM2DBL(size));
  if (argc == 3) {
    // TODO support extra font config options
  }

  NkRbFont *font = malloc(sizeof(NkRbFont));
  memset(font, 0, sizeof(NkRbFont));
  nk_font_atlas_init_default(&(font->atlas));
  nk_font_atlas_begin(&(font->atlas));
  font->font = nk_font_atlas_add_from_file(&(font->atlas), StringValueCStr(path), (float) NUM2DBL(size), NULL);
  if (font->font == NULL)
    rb_raise(rb_eStandardError, "Failed to load font file %s", StringValueCStr(path));
  font->img.data = nk_font_atlas_bake(&(font->atlas), &(font->img.width), &(font->img.height), NK_FONT_ATLAS_RGBA32);
  VALUE bytes = rb_str_new((char *) font->img.data, font->img.width * font->img.height * 4);
  VALUE tex = rb_yield_values(3, INT2FIX(font->img.width), INT2FIX(font->img.height), bytes);
  nk_font_atlas_end(&(font->atlas), nk_handle_ptr((void *) NUM2ULL(tex)), &(font->null));
  return Data_Wrap_Struct(cNuklearFont, NULL, nkrb_font_free, font);
}

void nkrb_font_init(void) {
  cNuklearFont = rb_define_class_under(mNuklear, "Font", rb_cObject);
  cNuklearNullTexture = rb_define_class_under(mNuklear, "NullTexture", rb_cObject);
  rb_define_module_function(cNuklearFont, "new", nkrb_font_new, -1);
  rb_define_method(cNuklearFont, "width", nkrb_font_width, 1);
  rb_define_method(cNuklearFont, "height", nkrb_font_height, 0);
}
