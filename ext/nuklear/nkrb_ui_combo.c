#include "nkrb.h"

VALUE nkrb_ui_combo(VALUE self, VALUE rcontext, VALUE rselected_index,
                    VALUE rchoices, VALUE ritem_height, VALUE rwidth, VALUE rheight) {
  struct nk_context *context = nkrb_context_get(rcontext);
  float width = (float) NUM2DBL(rwidth);
  float height = (float) NUM2DBL(rheight);
  int item_height = FIX2INT(ritem_height);
  int len = (int) RARRAY_LEN(rchoices);
  int selected_index = FIX2INT(rselected_index);
  int i;
  const char **choices = malloc(len * sizeof(const char *));
  for (i = 0; i < len; i++) {
    VALUE str = rb_funcall(rb_ary_entry(rchoices, i), rb_intern("to_s"), 0);
    choices[i] = StringValueCStr(str);
  }
  int result = nk_combo(context, choices, len, selected_index, item_height, nk_vec2(width, height));
  free(choices);
  return INT2FIX(result);
}

VALUE nkrb_ui_combobox(VALUE self, VALUE rcontext, VALUE rtext, VALUE rcolor,
                       VALUE rsymbol, VALUE rimage, VALUE rwidth, VALUE rheight) {
  struct nk_context *context = nkrb_context_get(rcontext);
  float width = (float) NUM2DBL(rwidth);
  float height = (float) NUM2DBL(rheight);
  int result = 0;
  enum nk_symbol_type symbol = NK_SYMBOL_NONE;
  SET_RSYMBOL(symbol, rsymbol);
  struct nk_vec2 size = nk_vec2(width, height);

  if (!NIL_P(rcolor)) {
    struct nk_color color;
    ASSIGN_STYLE_AS_COLOR(color, nkrb_style_item(rcolor));
    result = nk_combo_begin_color(context, color, nk_vec2(width, height));
  } else if (!NIL_P(rtext)) {
    rtext = rb_funcall(rtext, rb_intern("to_s"), 0);
    const char *text = StringValuePtr(rtext);
    int len = (int) RSTRING_LEN(rtext);
    if (!NIL_P(rsymbol)) {
      result = nk_combo_begin_symbol_text(context, text, len, symbol, size);
    } else if (!NIL_P(rimage)) {
      struct nk_image img;
      ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage));
      result = nk_combo_begin_image_text(context, text, len, img, size);
    } else {
      result = nk_combo_begin_text(context, text, len, size);
    }
  } else {
    if (!NIL_P(symbol)) {
      result = nk_combo_begin_symbol(context, symbol, size);
    } else if (!NIL_P(rimage)) {
      struct nk_image img;
      ASSIGN_STYLE_AS_IMAGE(img, nkrb_style_item(rimage));
      result = nk_combo_begin_image(context, img, size);
    } else {
      return Qnil;
    }
  }

  if (rb_block_given_p())
    rb_yield(result ? Qtrue : Qfalse);
  
  if (result)
    nk_combo_end(context);

  return Qnil;
}

void nkrb_ui_combo_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_combobox", nkrb_ui_combobox, 7);
  rb_define_method(mNuklearUIContainer, "ui_combo",    nkrb_ui_combo,    6);
}
