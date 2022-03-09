#include "nkrb.h"

#define UNPACK_CONTEXT(varname) struct nk_context *varname = nkrb_context_get(rb_ivar_get(self, rb_intern("@context")));

VALUE cNuklearUIBuilder = Qnil;

VALUE nkrb_ui_initialize(VALUE self, VALUE context) {
  rb_ivar_set(self, rb_intern("@context"), context);
  return self;
}


VALUE nkrb_ui_list_view_begin(VALUE self, VALUE id, VALUE flags, VALUE row_height, VALUE row_count) {
  UNPACK_CONTEXT(ctx);
  struct nk_list_view view;
  if (nk_list_view_begin(ctx, &view, StringValueCStr(id), FIX2INT(flags), FIX2INT(row_height), FIX2INT(row_count))) {
    if (rb_block_given_p()) {
      instance_eval_block(self);
    }
    nk_list_view_end(&view);
  }
  return self;
}

void nkrb_ui_builder_init(void) {
  cNuklearUIBuilder = rb_define_class_under(mNuklear, "UIBuilder", rb_cObject);
  rb_define_method(cNuklearUIBuilder, "initialize",                   nkrb_ui_initialize,                        1);
  rb_define_method(cNuklearUIBuilder, "list_view",                    nkrb_ui_list_view_begin,                   4);
}
