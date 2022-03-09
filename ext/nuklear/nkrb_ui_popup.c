#include "nkrb.h"

VALUE nkrb_ui_popup(VALUE self, VALUE context, VALUE rtype, VALUE rtitle, VALUE rflags, VALUE left, VALUE top, VALUE width, VALUE height) {
  struct nk_context *ctx = nkrb_context_get(context);
  const char *title = StringValueCStr(rtitle);
  int type = FIX2INT(rtype);
  int flags = FIX2INT(rflags);
  float x = (float) NUM2DBL(left);
  float y = (float) NUM2DBL(top);
  float w = (float) NUM2DBL(width);
  float h = (float) NUM2DBL(height);
  if (nk_popup_begin(ctx, type, title, flags, nk_rect(x, y, w, h))) {
    if (rb_block_given_p()) {
      instance_eval_block(ID2SYM(rb_intern("visible")));
    }
    nk_popup_end(ctx);
    return Qnil;
  }
  return ID2SYM(rb_intern("hidden"));
}

void nkrb_ui_popup_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_popup", nkrb_ui_popup, 8);
}
