#include "nkrb.h"

VALUE cNuklearUIWidget = Qnil;

void nkrb_ui_widget_init(void) {
  cNuklearUIWidget = rb_define_class_under(mNuklearUI, "Widget", rb_cObject);
}
