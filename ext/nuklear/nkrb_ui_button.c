#include "nkrb.h"

VALUE nkrb_ui_button(VALUE self, VALUE rcontext, VALUE roptions) {
  struct nk_context *context = nkrb_context_get(rcontext);
  VALUE rrepeat = rb_hash_aref(roptions, ID2SYM(rb_intern("repeat")));
  VALUE rtitle = rb_hash_aref(roptions, ID2SYM(rb_intern("title")));
  VALUE rcolor = rb_hash_aref(roptions, ID2SYM(rb_intern("color")));
  VALUE rsymbol = rb_hash_aref(roptions, ID2SYM(rb_intern("symbol")));
  VALUE rimage = rb_hash_aref(roptions, ID2SYM(rb_intern("image")));
  VALUE rstyle = rb_hash_aref(roptions, ID2SYM(rb_intern("style")));
  VALUE ralign = rb_hash_aref(roptions, ID2SYM(rb_intern("alignment")));
  int alignment = 0;
  if (!NIL_P(ralign)) alignment = FIX2INT(ralign);

  struct nk_style_button style;
  memcpy(&style, &context->style.button, sizeof(style));
  assign_button_style_from_ruby(&style, rstyle);

  enum nk_symbol_type symbol = NK_SYMBOL_NONE;
  SET_RSYMBOL(symbol, rsymbol);

  int repeat = RTEST(rrepeat);
  int result = 0;
  if (repeat) nk_button_set_behavior(context, NK_BUTTON_REPEATER);
  if (NIL_P(rtitle) && symbol == NK_SYMBOL_NONE && NIL_P(rimage)) {
    if (NIL_P(rcolor)) {
      rb_raise(rb_eArgError, "need :title or :symbol, with optional :image, or :color");
    } else {
      struct nk_color color;
      ASSIGN_STYLE_AS_COLOR(color, nkrb_style_item(rcolor));
      result = nk_button_color(context, color);
    }
  } else {
    if (NIL_P(rtitle) && NIL_P(rimage)) {
      result = nk_button_symbol_styled(context, &style, symbol);
    } else if (!NIL_P(rtitle) && NIL_P(rimage)) {
      result = nk_button_symbol_text_styled(context, &style, symbol, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), alignment);
    } else if (NIL_P(rtitle) && !NIL_P(rimage)) {
      struct nk_image img;
      ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage));
      result = nk_button_image_text_styled(context, &style, img, RSTRING_PTR(rtitle), (int) RSTRING_LEN(rtitle), alignment);
    } else {
      struct nk_image img;
      ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage));
      result = nk_button_image_styled(context, &style, img);
    }
  }
  if (repeat) nk_button_set_behavior(context, NK_BUTTON_DEFAULT);

  return INT2FIX(result);
}

void nkrb_ui_button_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_button", nkrb_ui_button, 2);
}
