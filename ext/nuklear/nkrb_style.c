#include "nkrb.h"

VALUE mNuklearStyle = Qnil;

struct nk_style_item *nkrb_style_item(VALUE value) {
  struct nk_style_item *item;
  Data_Get_Struct(value, struct nk_style_item, item);
  return item;
}

void nkrb_style_item_free(struct nk_style_item *item) {
  free(item);
}

void assign_button_style_from_ruby(struct nk_style_button *style, VALUE rstyle) {
  if (!NIL_P(rstyle)) {
    VALUE rnormal          = rb_hash_aref(rstyle, ID2SYM(rb_intern("normal")));
    VALUE rhover           = rb_hash_aref(rstyle, ID2SYM(rb_intern("hover")));
    VALUE ractive          = rb_hash_aref(rstyle, ID2SYM(rb_intern("active")));
    VALUE rborder_color    = rb_hash_aref(rstyle, ID2SYM(rb_intern("border_color")));
    VALUE rtext_background = rb_hash_aref(rstyle, ID2SYM(rb_intern("text_background")));
    VALUE rtext_normal     = rb_hash_aref(rstyle, ID2SYM(rb_intern("text_normal")));
    VALUE rtext_hover      = rb_hash_aref(rstyle, ID2SYM(rb_intern("text_hover")));
    VALUE rtext_active     = rb_hash_aref(rstyle, ID2SYM(rb_intern("text_active")));
    VALUE rtext_alignment  = rb_hash_aref(rstyle, ID2SYM(rb_intern("text_alignment")));
    VALUE rborder          = rb_hash_aref(rstyle, ID2SYM(rb_intern("border")));
    VALUE rrounding        = rb_hash_aref(rstyle, ID2SYM(rb_intern("rounding")));
    VALUE rpadding         = rb_hash_aref(rstyle, ID2SYM(rb_intern("padding")));
    VALUE rimage_padding   = rb_hash_aref(rstyle, ID2SYM(rb_intern("image_padding")));
    VALUE rtouch_padding   = rb_hash_aref(rstyle, ID2SYM(rb_intern("touch_padding")));
    
    // TODO button style user callbacks
    // nk_handle userdata;
    // void(*draw_begin)(struct nk_command_buffer*, nk_handle userdata);
    // void(*draw_end)(struct nk_command_buffer*, nk_handle userdata);

    if (!NIL_P(rnormal))          style->normal = *nkrb_style_item(rnormal);
    if (!NIL_P(rhover))           style->hover  = *nkrb_style_item(rhover);
    if (!NIL_P(ractive))          style->active = *nkrb_style_item(ractive);
    if (!NIL_P(rborder_color))    { ASSIGN_STYLE_AS_COLOR(style->border_color,    nkrb_style_item(rborder_color)); }
    if (!NIL_P(rtext_background)) { ASSIGN_STYLE_AS_COLOR(style->text_background, nkrb_style_item(rtext_background)); }
    if (!NIL_P(rtext_normal))     { ASSIGN_STYLE_AS_COLOR(style->text_normal,     nkrb_style_item(rtext_normal)); }
    if (!NIL_P(rtext_hover))      { ASSIGN_STYLE_AS_COLOR(style->text_hover,      nkrb_style_item(rtext_hover)); }
    if (!NIL_P(rtext_active))     { ASSIGN_STYLE_AS_COLOR(style->text_active,     nkrb_style_item(rtext_active)); }
    if (!NIL_P(rtext_alignment))  style->text_alignment = FIX2INT(rtext_alignment);
    if (!NIL_P(rborder))          style->border = (float) RFLOAT_VALUE(rborder);
    if (!NIL_P(rrounding))        style->rounding = (float) RFLOAT_VALUE(rrounding);
    if (!NIL_P(rpadding))         style->padding = nk_vec2(RFLOAT_VALUE(rb_ary_entry(rpadding, 0)),
                                                          RFLOAT_VALUE(rb_ary_entry(rpadding, 1)));
    if (!NIL_P(rimage_padding))   style->image_padding = nk_vec2(RFLOAT_VALUE(rb_ary_entry(rimage_padding, 0)),
                                                                 RFLOAT_VALUE(rb_ary_entry(rimage_padding, 1)));
    if (!NIL_P(rtouch_padding))   style->touch_padding = nk_vec2(RFLOAT_VALUE(rb_ary_entry(rtouch_padding, 0)),
                                                                 RFLOAT_VALUE(rb_ary_entry(rtouch_padding, 1)));
  }
}

void nkrb_style_init(void) {
  mNuklearStyle = rb_define_module_under(mNuklear, "Style");
  nkrb_style_color_init();
  nkrb_style_image_init();
}
