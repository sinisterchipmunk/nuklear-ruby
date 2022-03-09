#include "nkrb.h"

#define NK_UNPACK(rvalue, varname)                        \
  struct nk_style_item *varname;                          \
  Data_Get_Struct(rvalue, struct nk_style_item, varname);


VALUE cNuklearStyleColor = Qnil;

VALUE nkrb_style_color_alloc(VALUE self) {
  struct nk_style_item *style = malloc(sizeof(struct nk_style_item));
  memset(style, 0, sizeof(struct nk_style_item));
  style->type = NK_STYLE_ITEM_COLOR;
  return Data_Wrap_Struct(self, NULL, nkrb_style_item_free, style);
}

VALUE nkrb_style_color_set_red(VALUE self, VALUE r) {
  NK_UNPACK(self, style);
  style->data.color.r = (nk_byte) (NK_SATURATE((float) NUM2DBL(r)) * 255.0f);
  return r;
}

VALUE nkrb_style_color_get_red(VALUE self) {
  NK_UNPACK(self, style);
  return DBL2NUM(style->data.color.r / 255.0);
}

VALUE nkrb_style_color_set_green(VALUE self, VALUE g) {
  NK_UNPACK(self, style);
  style->data.color.g = (nk_byte) (NK_SATURATE((float) NUM2DBL(g)) * 255.0f);
  return g;
}

VALUE nkrb_style_color_get_green(VALUE self) {
  NK_UNPACK(self, style);
  return DBL2NUM(style->data.color.g / 255.0);
}

VALUE nkrb_style_color_set_blue(VALUE self, VALUE b) {
  NK_UNPACK(self, style);
  style->data.color.b = (nk_byte) (NK_SATURATE((float) NUM2DBL(b)) * 255.0f);
  return b;
}

VALUE nkrb_style_color_get_blue(VALUE self) {
  NK_UNPACK(self, style);
  return DBL2NUM(style->data.color.b / 255.0);
}

VALUE nkrb_style_color_set_alpha(VALUE self, VALUE a) {
  NK_UNPACK(self, style);
  style->data.color.a = (nk_byte) (NK_SATURATE((float) NUM2DBL(a)) * 255.0f);
  return a;
}

VALUE nkrb_style_color_get_alpha(VALUE self) {
  NK_UNPACK(self, style);
  return DBL2NUM(style->data.color.a / 255.0);
}

VALUE nkrb_style_color_set_hue(VALUE self, VALUE a) {
  NK_UNPACK(self, style);
  nk_byte tmp[4];
  nk_color_hsva_bv(tmp, style->data.color);
  tmp[0] = (nk_byte) (NK_SATURATE((float) NUM2DBL(a)) * 255.0f);
  style->data.color = nk_hsva_bv(tmp);
  return a;
}

VALUE nkrb_style_color_get_hue(VALUE self) {
  NK_UNPACK(self, style);
  nk_byte tmp[4];
  nk_color_hsva_bv(tmp, style->data.color);
  return DBL2NUM(tmp[0] / 255.0);
}

VALUE nkrb_style_color_set_saturation(VALUE self, VALUE a) {
  NK_UNPACK(self, style);
  nk_byte tmp[4];
  nk_color_hsva_bv(tmp, style->data.color);
  tmp[1] = (nk_byte) (NK_SATURATE((float) NUM2DBL(a)) * 255.0f);
  style->data.color = nk_hsva_bv(tmp);
  return a;
}

VALUE nkrb_style_color_get_saturation(VALUE self) {
  NK_UNPACK(self, style);
  nk_byte tmp[4];
  nk_color_hsva_bv(tmp, style->data.color);
  return DBL2NUM(tmp[1] / 255.0);
}

VALUE nkrb_style_color_set_value(VALUE self, VALUE a) {
  NK_UNPACK(self, style);
  nk_byte tmp[4];
  nk_color_hsva_bv(tmp, style->data.color);
  tmp[3] = (nk_byte) (NK_SATURATE((float) NUM2DBL(a)) * 255.0f);
  style->data.color = nk_hsva_bv(tmp);
  return a;
}

VALUE nkrb_style_color_get_value(VALUE self) {
  NK_UNPACK(self, style);
  nk_byte tmp[4];
  nk_color_hsva_bv(tmp, style->data.color);
  return DBL2NUM(tmp[3] / 255.0);
}

void nkrb_style_color_init(void) {
  cNuklearStyleColor = rb_define_class_under(mNuklearStyle, "Color", rb_cObject);
  rb_define_alloc_func(cNuklearStyleColor, nkrb_style_color_alloc);
  rb_define_method(cNuklearStyleColor, "red=",   nkrb_style_color_set_red, 1);
  rb_define_method(cNuklearStyleColor, "red",    nkrb_style_color_get_red, 0);
  rb_define_method(cNuklearStyleColor, "green=", nkrb_style_color_set_green, 1);
  rb_define_method(cNuklearStyleColor, "green",  nkrb_style_color_get_green, 0);
  rb_define_method(cNuklearStyleColor, "blue=",  nkrb_style_color_set_blue, 1);
  rb_define_method(cNuklearStyleColor, "blue",   nkrb_style_color_get_blue, 0);
  rb_define_method(cNuklearStyleColor, "hue=",   nkrb_style_color_set_hue, 1);
  rb_define_method(cNuklearStyleColor, "hue",    nkrb_style_color_get_hue, 0);
  rb_define_method(cNuklearStyleColor, "saturation=", nkrb_style_color_set_saturation, 1);
  rb_define_method(cNuklearStyleColor, "saturation",  nkrb_style_color_get_saturation, 0);
  rb_define_method(cNuklearStyleColor, "value=",  nkrb_style_color_set_value, 1);
  rb_define_method(cNuklearStyleColor, "value",   nkrb_style_color_get_value, 0);
  rb_define_method(cNuklearStyleColor, "alpha=", nkrb_style_color_set_alpha, 1);
  rb_define_method(cNuklearStyleColor, "alpha",  nkrb_style_color_get_alpha, 0);
}
