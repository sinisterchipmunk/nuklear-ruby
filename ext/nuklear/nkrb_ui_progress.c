#include "nkrb.h"

VALUE nkrb_ui_progress(VALUE self, VALUE context, VALUE rcurrent, VALUE rmax, VALUE rmodifiable) {
  struct nk_context *ctx = nkrb_context_get(context);
  nk_size current = (nk_size) NUM2LL(rcurrent);
  nk_size max     = (nk_size) NUM2LL(rmax);
  int modifiable  = nk_false;
  if (RTEST(rmodifiable)) modifiable = nk_true;

  if (nk_progress(ctx, &current, max, modifiable)) {
    return LL2NUM(current);
  }

  return Qnil;
}

void nkrb_ui_progress_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_progress", nkrb_ui_progress, 4);
}
