#include "nkrb.h"

VALUE nkrb_ui_group(VALUE self, VALUE rcontext, VALUE rtitle, VALUE rflags, VALUE rscroll_x, VALUE rscroll_y) {
  struct nk_context *context = nkrb_context_get(rcontext);
  const char *title = StringValueCStr(rtitle);
  int flags = FIX2INT(rflags);
  unsigned int scroll_x = (unsigned int) FIX2INT(rscroll_x);
  unsigned int scroll_y = (unsigned int) FIX2INT(rscroll_y);
  int result = nk_group_scrolled_offset_begin(context, &scroll_x, &scroll_y, title, flags);

  if (result) {
    if (rb_block_given_p()) {
      instance_eval_block(self);
    }
    nk_group_end(context);
    VALUE scroll = rb_ary_new();
    rb_ary_push(scroll, INT2FIX(scroll_x));
    rb_ary_push(scroll, INT2FIX(scroll_y));
    return scroll;
  }

  return Qnil;
}

void nkrb_ui_group_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_group", nkrb_ui_group, 5);
}
