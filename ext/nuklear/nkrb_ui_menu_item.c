#include "nkrb.h"

VALUE nkrb_ui_menu_item(VALUE self, VALUE rcontext, VALUE roptions) {
  struct nk_context *context = nkrb_context_get(rcontext);

  VALUE rtitle = rb_funcall(rb_hash_aref(roptions, ID2SYM(rb_intern("title"))), rb_intern("to_s"), 0);
  VALUE rimage = rb_hash_aref(roptions, ID2SYM(rb_intern("image")));
  VALUE rsymbol = rb_hash_aref(roptions, ID2SYM(rb_intern("symbol")));
  nk_flags align = FIX2INT(rb_hash_aref(roptions, ID2SYM(rb_intern("align"))));

  enum nk_symbol_type symbol = NK_SYMBOL_NONE;
  SET_RSYMBOL(symbol, rsymbol);

  int result = 0;
  if (!NIL_P(rimage)) {
    struct nk_image img;
    ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage))
    result = nk_menu_item_image_text(context, img, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), align);
  } else if (!NIL_P(rsymbol)) {
    result = nk_menu_item_symbol_text(context, symbol, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), align);
  } else {
    result = nk_menu_item_text(context, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), align);
  }

  return result ? ID2SYM(rb_intern("clicked")) : Qnil;
}

void nkrb_ui_menu_item_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_menu_item", nkrb_ui_menu_item, 2);
}
