#include "nkrb.h"

VALUE nkrb_ui_layout_row(VALUE self, VALUE rcontext, VALUE rformat, VALUE rheight, VALUE rratios) {
  struct nk_context *ctx = nkrb_context_get(rcontext);
  enum nk_layout_format fmt = NK_DYNAMIC;
  if (SYM2ID(rformat) == rb_intern("static")) fmt = NK_STATIC;
  float height = (float) NUM2DBL(rheight);
  int len = (int) RARRAY_LEN(rratios);
  float *ratios = malloc(len * sizeof(float));
  int i;
  for (i = 0; i < len; i++) {
    ratios[i] = (float) NUM2DBL(rb_ary_entry(rratios, i));
  }
  nk_layout_row(ctx, fmt, height, len, ratios);
  free(ratios);
  return Qnil;
}

VALUE nkrb_ui_layout_row_dynamic(VALUE self, VALUE context, VALUE height, VALUE ncols) {
  nk_layout_row_dynamic(nkrb_context_get(context), (float) NUM2DBL(height), FIX2INT(ncols));
  return Qtrue;
}

VALUE nkrb_ui_layout_row_static(VALUE self, VALUE context, VALUE height, VALUE col_width, VALUE ncols) {
  nk_layout_row_static(nkrb_context_get(context), (float) NUM2DBL(height), FIX2INT(col_width), FIX2INT(ncols));
  return Qtrue;
}

VALUE nkrb_ui_layout_row_begin(VALUE self, VALUE context, VALUE format, VALUE row_height, VALUE ncols) {
  struct nk_context *ctx = nkrb_context_get(context);
  enum nk_layout_format fmt = NK_DYNAMIC;
  if (SYM2ID(format) == rb_intern("static")) fmt = NK_STATIC;
  nk_layout_row_begin(ctx, fmt, (float) NUM2DBL(row_height), FIX2INT(ncols));
  if (rb_block_given_p()) {
    instance_eval_block(Qtrue);
  }
  nk_layout_row_end(ctx);
  return Qnil;
}

VALUE nkrb_ui_layout_row_push(VALUE self, VALUE context, VALUE value) {
  nk_layout_row_push(nkrb_context_get(context), (float) NUM2DBL(value));
  return Qtrue;
}

VALUE nkrb_ui_layout_row_template_begin(VALUE self, VALUE context, VALUE height) {
  nk_layout_row_template_begin(nkrb_context_get(context), (float) NUM2DBL(height));
  return Qnil;
}

VALUE nkrb_ui_layout_row_template_push_dynamic(VALUE self, VALUE context) {
  nk_layout_row_template_push_dynamic(nkrb_context_get(context));
  return Qnil;
}

VALUE nkrb_ui_layout_row_template_push_variable(VALUE self, VALUE context, VALUE min_width) {
  nk_layout_row_template_push_variable(nkrb_context_get(context), (float) NUM2DBL(min_width));
  return Qnil;
}

VALUE nkrb_ui_layout_row_template_push_static(VALUE self, VALUE context, VALUE width) {
  nk_layout_row_template_push_static(nkrb_context_get(context), (float) NUM2DBL(width));
  return Qnil;
}

VALUE nkrb_ui_layout_row_template_end(VALUE self, VALUE context) {
  nk_layout_row_template_end(nkrb_context_get(context));
  return Qnil;
}

VALUE nkrb_ui_layout_space_begin(VALUE self, VALUE context, VALUE format, VALUE height, VALUE widget_count) {
  enum nk_layout_format fmt = NK_DYNAMIC;
  if (SYM2ID(format) == rb_intern("static")) fmt = NK_STATIC;
  nk_layout_space_begin(nkrb_context_get(context), fmt, (float) NUM2DBL(height), FIX2INT(widget_count));
  return Qnil;
}

VALUE nkrb_ui_layout_space_push(VALUE self, VALUE context, VALUE rbounds) {
  VALUE left   = rb_hash_aref(rbounds, ID2SYM(rb_intern("left")));
  VALUE top    = rb_hash_aref(rbounds, ID2SYM(rb_intern("top")));
  VALUE width  = rb_hash_aref(rbounds, ID2SYM(rb_intern("width")));
  VALUE height = rb_hash_aref(rbounds, ID2SYM(rb_intern("height")));
  if (NIL_P(left))   rb_raise(rb_eArgError, ":left option is required");
  if (NIL_P(top))    rb_raise(rb_eArgError, ":top option is required");
  if (NIL_P(width))  rb_raise(rb_eArgError, ":width option is required");
  if (NIL_P(height)) rb_raise(rb_eArgError, ":height option is required");
  float x = (float) NUM2DBL(left);
  float y = (float) NUM2DBL(top);
  float w = (float) NUM2DBL(width);
  float h = (float) NUM2DBL(height);
  nk_layout_space_push(nkrb_context_get(context), nk_rect(x, y, w, h));
  return Qnil;
}

VALUE nkrb_ui_layout_space_end(VALUE self, VALUE context) {
  nk_layout_space_end(nkrb_context_get(context));
  return Qnil;
}

VALUE nkrb_ui_layout_widget_bounds(VALUE self, VALUE context) {
  struct nk_rect bounds = nk_layout_widget_bounds(nkrb_context_get(context));
  VALUE ret = rb_hash_new();
  rb_hash_aset(ret, ID2SYM(rb_intern("left")),   bounds.x);
  rb_hash_aset(ret, ID2SYM(rb_intern("top")),    bounds.y);
  rb_hash_aset(ret, ID2SYM(rb_intern("width")),  bounds.w);
  rb_hash_aset(ret, ID2SYM(rb_intern("height")), bounds.h);
  return Qnil;
}

void nkrb_ui_layout_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_layout_row",                   nkrb_ui_layout_row,                        4);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_dynamic",           nkrb_ui_layout_row_dynamic,                3);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_static",            nkrb_ui_layout_row_static,                 4);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_begin",             nkrb_ui_layout_row_begin,                  4);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_push",              nkrb_ui_layout_row_push,                   2);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_template_begin",    nkrb_ui_layout_row_template_begin,         2);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_template_dynamic",  nkrb_ui_layout_row_template_push_dynamic,  1);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_template_variable", nkrb_ui_layout_row_template_push_variable, 2);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_template_static",   nkrb_ui_layout_row_template_push_static,   2);
  rb_define_method(mNuklearUIContainer, "ui_layout_row_template_end",      nkrb_ui_layout_row_template_end,           1);
  rb_define_method(mNuklearUIContainer, "ui_layout_space_begin",           nkrb_ui_layout_space_begin,                4);
  rb_define_method(mNuklearUIContainer, "ui_layout_space_push",            nkrb_ui_layout_space_push,                 2);
  rb_define_method(mNuklearUIContainer, "ui_layout_space_end",             nkrb_ui_layout_space_end,                  1);
  rb_define_method(mNuklearUIContainer, "ui_layout_widget_bounds",         nkrb_ui_layout_widget_bounds,              1);
}
