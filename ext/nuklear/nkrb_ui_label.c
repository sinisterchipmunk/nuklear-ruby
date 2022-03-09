#include "nkrb.h"

VALUE nkrb_ui_label(VALUE self, VALUE rcontext, VALUE text, VALUE rcolor, VALUE rwrap, VALUE ralign) {
  struct nk_context *context = nkrb_context_get(rcontext);
  int align = FIX2INT(ralign);

  if (RTEST(rwrap)) {
    if (NIL_P(rcolor)) {
      nk_text_wrap(context, StringValuePtr(text), (int) RSTRING_LEN(text));
    } else {
      struct nk_color color;
      ASSIGN_STYLE_AS_COLOR(color, nkrb_style_item(rcolor));
      nk_text_wrap_colored(context, StringValuePtr(text), (int) RSTRING_LEN(text), color);
    }
  } else {
    if (NIL_P(rcolor)) {
      nk_text(context, StringValuePtr(text), (int) RSTRING_LEN(text), align);
    } else {
      struct nk_color color;
      ASSIGN_STYLE_AS_COLOR(color, nkrb_style_item(rcolor));
      nk_text_colored(context, StringValuePtr(text), (int) RSTRING_LEN(text), align, color);
    }
  }

  return Qnil;
}

void nkrb_ui_label_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_label", nkrb_ui_label, 5);
}
