#include "nkrb.h"

VALUE nkrb_ui_slider(VALUE self, VALUE context, VALUE rcurrent, VALUE rmin, VALUE rmax, VALUE rstep) {
  struct nk_context *ctx = nkrb_context_get(context);
  float current = (float) NUM2DBL(rcurrent);
  float minimum = (float) NUM2DBL(rmin);
  float maximum = (float) NUM2DBL(rmax);
  float step    = (float) NUM2DBL(rstep);

  if (nk_slider_float(ctx, minimum, &current, maximum, step)) {
    return DBL2NUM(current);
  }

  return Qnil;
}

void nkrb_ui_slider_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_slider", nkrb_ui_slider, 5);
}
