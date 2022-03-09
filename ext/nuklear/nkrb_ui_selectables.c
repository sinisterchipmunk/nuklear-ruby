#include "nkrb.h"

VALUE nkrb_ui_checkbox(VALUE self, VALUE rcontext, VALUE rtext, VALUE ris_checked) {
  struct nk_context *context = nkrb_context_get(rcontext);
  const char *text = StringValueCStr(rtext);
  int is_checked = RTEST(ris_checked) ? 1 : 0;
  if (nk_checkbox_label(context, text, &is_checked)) {
    return is_checked ? Qtrue : Qfalse;
  }
  return Qnil;
}

VALUE nkrb_ui_option(VALUE self, VALUE rcontext, VALUE rtext, VALUE ris_checked) {
  struct nk_context *context = nkrb_context_get(rcontext);
  const char *text = StringValueCStr(rtext);
  int is_checked = RTEST(ris_checked) ? 1 : 0;
  if (nk_option_label(context, text, is_checked)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE nkrb_ui_selectable(VALUE self, VALUE rcontext, VALUE rimage, VALUE rtext, VALUE rselected, VALUE ralign) {
  struct nk_context *context = nkrb_context_get(rcontext);
  int align = FIX2INT(ralign);
  int selected = RTEST(rselected) ? 1 : 0;
  int result;
  if (NIL_P(rtext)) return Qnil;

  const char *text = StringValuePtr(rtext);
  int len = (int) RSTRING_LEN(rtext);

  if (NIL_P(rimage))
    result = nk_selectable_text(context, text, len, align, &selected);
  else {
    struct nk_image img;
    ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage))
    result = nk_selectable_image_text(context, img, text, len, align, &selected);
  }

  if (result) {
    return selected ? Qtrue : Qfalse;
  }

  return Qnil;
}

void nkrb_ui_checkbox_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_checkbox",   nkrb_ui_checkbox,   3);
  rb_define_method(mNuklearUIContainer, "ui_option",     nkrb_ui_option,     3);
  rb_define_method(mNuklearUIContainer, "ui_selectable", nkrb_ui_selectable, 5);
}
