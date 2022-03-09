#include "nkrb.h"

VALUE nkrb_ui_window(VALUE self, VALUE context, VALUE rid, VALUE rtitle, VALUE rflags, VALUE left, VALUE top, VALUE width, VALUE height) {
  struct nk_context *ctx = nkrb_context_get(context);
  const char *id = StringValueCStr(rid);
  const char *title = StringValueCStr(rtitle);
  int flags = FIX2INT(rflags);
  float x = (float) NUM2DBL(left);
  float y = (float) NUM2DBL(top);
  float w = (float) NUM2DBL(width);
  float h = (float) NUM2DBL(height);
  if (nk_begin_titled(ctx, id, title, nk_rect(x, y, w, h), flags)) {
    VALUE result = rb_ary_new();
    struct nk_rect bounds         = nk_window_get_bounds(ctx);
    struct nk_rect content_region = nk_window_get_content_region(ctx);
    rb_ary_push(result, DBL2NUM(bounds.x));
    rb_ary_push(result, DBL2NUM(bounds.y));
    rb_ary_push(result, DBL2NUM(bounds.w));
    rb_ary_push(result, DBL2NUM(bounds.h));
    rb_ary_push(result, DBL2NUM(content_region.x));
    rb_ary_push(result, DBL2NUM(content_region.y));
    rb_ary_push(result, DBL2NUM(content_region.w));
    rb_ary_push(result, DBL2NUM(content_region.h));

    if (rb_block_given_p()) {
      rb_yield(result);
    }
  }
  nk_end(ctx);
  return Qnil;
}

VALUE nkrb_ui_window_close(VALUE self, VALUE context, VALUE rid) {
  struct nk_context *ctx = nkrb_context_get(context);
  const char *id = StringValueCStr(rid);
  nk_window_close(ctx, id);
  return Qnil;
}

void nkrb_ui_window_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_window", nkrb_ui_window, 8);
  rb_define_method(mNuklearUIContainer, "ui_window_close", nkrb_ui_window_close, 2);
}
