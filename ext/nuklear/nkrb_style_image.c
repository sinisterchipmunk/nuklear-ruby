#include "nkrb.h"

#define NK_UNPACK(rvalue, varname)                        \
  struct nk_style_item *varname;                          \
  Data_Get_Struct(rvalue, struct nk_style_item, varname);

VALUE cNuklearStyleImage = Qnil;

VALUE nkrb_style_image_alloc(VALUE self) {
  struct nk_style_item *style = malloc(sizeof(struct nk_style_item));
  memset(style, 0, sizeof(struct nk_style_item));
  style->type = NK_STYLE_ITEM_IMAGE;
  return Data_Wrap_Struct(self, NULL, nkrb_style_item_free, style);
}

VALUE nkrb_style_image_set_id(VALUE self, VALUE id) {
  NK_UNPACK(self, style);
  style->data.image.handle.id = FIX2INT(id);
  return id;
}

VALUE nkrb_style_image_get_id(VALUE self) {
  NK_UNPACK(self, style);
  return INT2FIX(style->data.image.handle.id);
}

void nkrb_style_image_init(void) {
  cNuklearStyleImage = rb_define_class_under(mNuklearStyle, "Image", rb_cObject);
  rb_define_alloc_func(cNuklearStyleImage, nkrb_style_image_alloc);
  rb_define_method(cNuklearStyleImage, "id=",   nkrb_style_image_set_id, 1);
  rb_define_method(cNuklearStyleImage, "id",    nkrb_style_image_get_id, 0);
}
