#include "nkrb.h"

VALUE nkrb_ui_menu(VALUE self, VALUE rcontext, VALUE roptions) {
  struct nk_context *context = nkrb_context_get(rcontext);

  VALUE rtitle = rb_hash_aref(roptions, ID2SYM(rb_intern("title")));
  VALUE rimage = rb_hash_aref(roptions, ID2SYM(rb_intern("image")));
  VALUE rsymbol = rb_hash_aref(roptions, ID2SYM(rb_intern("symbol")));
  nk_flags align = FIX2INT(rb_hash_aref(roptions, ID2SYM(rb_intern("align"))));
  float width  = (float) NUM2DBL(rb_hash_aref(roptions, ID2SYM(rb_intern("width"))));
  float height = (float) NUM2DBL(rb_hash_aref(roptions, ID2SYM(rb_intern("height"))));
  struct nk_vec2 size = nk_vec2(width, height);
  VALUE rid = rb_hash_aref(roptions, ID2SYM(rb_intern("id")));

  enum nk_symbol_type symbol = NK_SYMBOL_NONE;
  SET_RSYMBOL(symbol, rsymbol);

  int result = 0;
  if (!NIL_P(rimage)) {
    struct nk_image img;
    ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage))
    if (!NIL_P(rtitle)) {
      result = nk_menu_begin_image_text(context, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), align, img, size);
    } else {
      rid = rb_funcall(rid, rb_intern("to_s"), 0);
      result = nk_menu_begin_image(context, StringValueCStr(rid), img, size);
    }
  } else if (!NIL_P(rtitle)) {
    result = nk_menu_begin_symbol_text(context, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), align, symbol, size);
  } else {
    rid = rb_funcall(rid, rb_intern("to_s"), 0);
    result = nk_menu_begin_symbol(context, StringValueCStr(rid), symbol, size);
  }

  if (result) {
    if (rb_block_given_p()) {
      rb_yield(Qtrue);
    }
    nk_menu_end(context);
  } else {
    return Qfalse;
  }

  return Qnil;
}

void nkrb_ui_menu_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_menu", nkrb_ui_menu, 2);
}
