#include "nkrb.h"

VALUE nkrb_ui_color_picker(VALUE self, VALUE rcontext, VALUE rcolor, VALUE rtype) {
  struct nk_context *ctx = nkrb_context_get(rcontext);
  enum nk_color_format type = (SYM2ID(rtype) == rb_intern("rgb")) ? NK_RGB : NK_RGBA;
  struct nk_style_item *style = nkrb_style_item(rcolor);
  struct nk_color color;
  ASSIGN_STYLE_AS_COLOR(color, style);

  if (nk_color_pick(ctx, &color, type)) {
    style->data.color = color;
    return rcolor;
  }

  return Qnil;
}

void nkrb_ui_color_picker_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_color_picker", nkrb_ui_color_picker, 3);
}
