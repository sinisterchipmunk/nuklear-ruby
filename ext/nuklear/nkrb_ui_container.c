#include "nkrb.h"

VALUE mNuklearUIContainer = Qnil;

void nkrb_ui_container_init(void) {
  mNuklearUIContainer = rb_define_module_under(mNuklearUI, "Container");
}
