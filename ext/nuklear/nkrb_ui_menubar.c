#include "nkrb.h"

VALUE nkrb_ui_menubar(VALUE self, VALUE rcontext) {
  struct nk_context *context = nkrb_context_get(rcontext);

  nk_menubar_begin(context);

  if (rb_block_given_p()) {
    rb_yield(Qtrue);
  }

  nk_menubar_end(context);
  return Qnil;
}

void nkrb_ui_menubar_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_menubar", nkrb_ui_menubar, 1);
}
