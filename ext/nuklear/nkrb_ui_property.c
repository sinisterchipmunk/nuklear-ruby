#include "nkrb.h"

VALUE nkrb_ui_property(VALUE self, VALUE context, VALUE rid, VALUE rmin, VALUE rcurrent, VALUE rmax, VALUE rstep, VALUE rinc_per_pixel) {
  struct nk_context *ctx = nkrb_context_get(context);
  float min     = (float) NUM2DBL(rmin);
  float current = (float) NUM2DBL(rcurrent);
  float max     = (float) NUM2DBL(rmax);
  float step    = (float) NUM2DBL(rstep);
  float inc_pp  = (float) NUM2DBL(rinc_per_pixel);
  float prev = current;
  rid = rb_funcall(rid, rb_intern("to_s"), 0);

  nk_property_float(ctx, StringValueCStr(rid), min, &current, max, step, inc_pp);
  if (current != prev) return DBL2NUM(current);
  return Qnil;
}

void nkrb_ui_property_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_property", nkrb_ui_property, 7);
}
