#include "nkrb.h"

#define VALUE_TO_NK(value, varname)  \
  struct nk_context *varname = NULL; \
  Data_Get_Struct(value, struct nk_context, varname);

VALUE cNuklearContext = Qnil;
VALUE cNuklearContextEventSink = Qnil;

void nkrb_context_free(struct nk_context *ctx) {
  nk_free(ctx);
  free(ctx);
}

struct nk_context *nkrb_context_get(VALUE context) {
  VALUE_TO_NK(context, ctx);
  return ctx;
}

VALUE nkrb_context_alloc(VALUE self) {
  struct nk_context *ctx = malloc(sizeof(struct nk_context));
  memset(ctx, 0, sizeof(struct nk_context));
  return Data_Wrap_Struct(self, NULL, nkrb_context_free, ctx);
}

void set_default_style(struct nk_context *ctx) {
  struct nk_color table[NK_COLOR_COUNT];
  table[NK_COLOR_TEXT]                    = nk_rgba(210, 210, 210, 255);
  table[NK_COLOR_WINDOW]                  = nk_rgba( 57,  67,  71, 215);
  table[NK_COLOR_HEADER]                  = nk_rgba( 51,  51,  56, 220);
  table[NK_COLOR_BORDER]                  = nk_rgba( 46,  46,  46, 255);
  table[NK_COLOR_BUTTON]                  = nk_rgba( 48,  83, 111, 255);
  table[NK_COLOR_BUTTON_HOVER]            = nk_rgba( 58,  93, 121, 255);
  table[NK_COLOR_BUTTON_ACTIVE]           = nk_rgba( 63,  98, 126, 255);
  table[NK_COLOR_TOGGLE]                  = nk_rgba( 50,  58,  61, 255);
  table[NK_COLOR_TOGGLE_HOVER]            = nk_rgba( 45,  53,  56, 255);
  table[NK_COLOR_TOGGLE_CURSOR]           = nk_rgba( 48,  83, 111, 255);
  table[NK_COLOR_SELECT]                  = nk_rgba( 57,  67,  61, 255);
  table[NK_COLOR_SELECT_ACTIVE]           = nk_rgba( 48,  83, 111, 255);
  table[NK_COLOR_SLIDER]                  = nk_rgba( 50,  58,  61, 255);
  table[NK_COLOR_SLIDER_CURSOR]           = nk_rgba( 48,  83, 111, 245);
  table[NK_COLOR_SLIDER_CURSOR_HOVER]     = nk_rgba( 53,  88, 116, 255);
  table[NK_COLOR_SLIDER_CURSOR_ACTIVE]    = nk_rgba( 58,  93, 121, 255);
  table[NK_COLOR_PROPERTY]                = nk_rgba( 50,  58,  61, 255);
  table[NK_COLOR_EDIT]                    = nk_rgba( 50,  58,  61, 225);
  table[NK_COLOR_EDIT_CURSOR]             = nk_rgba(210, 210, 210, 255);
  table[NK_COLOR_COMBO]                   = nk_rgba( 50,  58,  61, 255);
  table[NK_COLOR_CHART]                   = nk_rgba( 50,  58,  61, 255);
  table[NK_COLOR_CHART_COLOR]             = nk_rgba( 48,  83, 111, 255);
  table[NK_COLOR_CHART_COLOR_HIGHLIGHT]   = nk_rgba(255,   0,   0, 255);
  table[NK_COLOR_SCROLLBAR]               = nk_rgba( 50,  58,  61, 255);
  table[NK_COLOR_SCROLLBAR_CURSOR]        = nk_rgba( 48,  83, 111, 255);
  table[NK_COLOR_SCROLLBAR_CURSOR_HOVER]  = nk_rgba( 53,  88, 116, 255);
  table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba( 58,  93, 121, 255);
  table[NK_COLOR_TAB_HEADER]              = nk_rgba( 48,  83, 111, 255);
  nk_style_from_table(ctx, table);
}

VALUE nkrb_context_initialize(VALUE context, VALUE font) {
  struct nk_context *ctx = nkrb_context_get(context);
  if (nk_init_default(ctx, nkrb_font_to_nk(font)) == 0) {
    rb_raise(rb_eStandardError, "Failed to initialize Nuklear context");
  }
  set_default_style(ctx);
  VALUE event_sink = rb_funcall(cNuklearContextEventSink, rb_intern("new"), 1, context);
  rb_ivar_set(context, rb_intern("@event_sink"), event_sink);
  rb_ivar_set(context, rb_intern("@font"), font);
  rb_ivar_set(context, rb_intern("@null"), nkrb_font_get_null(font));
  rb_ivar_set(context, rb_intern("@ui_builder"), rb_funcall(cNuklearUIBuilder, rb_intern("new"), 1, context));

  return context;
}

VALUE nkrb_context_set_user_data(VALUE self, VALUE userdata) {
  VALUE_TO_NK(self, ctx);
  // maybe come back to this, might be able to access user data later on through context
  // nk_set_user_data(ctx, userdata);
  rb_ivar_set(self, rb_intern("@userdata"), userdata);
  return userdata;
}

VALUE nkrb_context_get_user_data(VALUE self) {
  return rb_ivar_get(self, rb_intern("@userdata"));
}

VALUE nkrb_context_tick(VALUE self) {
  VALUE_TO_NK(self, ctx);
  nk_input_begin(ctx);
  rb_funcall(self, rb_intern("process_pending_events"), 0);
  nk_input_end(ctx);
  rb_funcall(self, rb_intern("paint"), 0);
  nk_clear(ctx);
  return self;
}

VALUE rb_nkrb_event_sink_initialize(VALUE self, VALUE context) {
  rb_ivar_set(self, rb_intern("@context"), context);
  return self;
}

VALUE nkrb_event_sink_motion(VALUE self, VALUE rx, VALUE ry, VALUE rxrel, VALUE ryrel) {
  VALUE_TO_NK(rb_ivar_get(self, rb_intern("@context")), ctx);
  if (ctx->input.mouse.grabbed) {
    int x = (int) ctx->input.mouse.prev.x, y = (int) ctx->input.mouse.prev.y;
    nk_input_motion(ctx, x + FIX2INT(rxrel), y + FIX2INT(ryrel));
  } else {
    nk_input_motion(ctx, FIX2INT(rx), FIX2INT(ry));
  }
  return self;
}

VALUE nkrb_event_sink_key(VALUE self, VALUE keys, VALUE down) {
  VALUE_TO_NK(rb_ivar_get(self, rb_intern("@context")), ctx);
  ID keys_id = SYM2ID(keys);
  int is_down = RTEST(down) ? 1 : 0;

  if (keys_id == rb_intern("shift"))          nk_input_key(ctx, NK_KEY_SHIFT, is_down);
  else if (keys_id == rb_intern("ctrl"))      nk_input_key(ctx, NK_KEY_CTRL, is_down);
  else if (keys_id == rb_intern("del"))       nk_input_key(ctx, NK_KEY_DEL, is_down);
  else if (keys_id == rb_intern("enter"))     nk_input_key(ctx, NK_KEY_ENTER, is_down);
  else if (keys_id == rb_intern("tab"))       nk_input_key(ctx, NK_KEY_TAB, is_down);
  else if (keys_id == rb_intern("backspace")) nk_input_key(ctx, NK_KEY_BACKSPACE, is_down);
  else if (keys_id == rb_intern("copy"))      nk_input_key(ctx, NK_KEY_COPY, is_down);
  else if (keys_id == rb_intern("cut"))       nk_input_key(ctx, NK_KEY_CUT, is_down);
  else if (keys_id == rb_intern("paste"))     nk_input_key(ctx, NK_KEY_PASTE, is_down);
  else if (keys_id == rb_intern("up"))        nk_input_key(ctx, NK_KEY_UP, is_down);
  else if (keys_id == rb_intern("down"))      nk_input_key(ctx, NK_KEY_DOWN, is_down);
  else if (keys_id == rb_intern("left"))      nk_input_key(ctx, NK_KEY_LEFT, is_down);
  else if (keys_id == rb_intern("right"))     nk_input_key(ctx, NK_KEY_RIGHT, is_down);

  /* Shortcuts: text field */
  else if (keys_id == rb_intern("text_insert_mode"))  nk_input_key(ctx, NK_KEY_TEXT_INSERT_MODE, is_down);
  else if (keys_id == rb_intern("text_replace_mode")) nk_input_key(ctx, NK_KEY_TEXT_REPLACE_MODE, is_down);
  else if (keys_id == rb_intern("text_reset_mode"))   nk_input_key(ctx, NK_KEY_TEXT_RESET_MODE, is_down);
  else if (keys_id == rb_intern("text_line_start"))   nk_input_key(ctx, NK_KEY_TEXT_LINE_START, is_down);
  else if (keys_id == rb_intern("text_line_end"))     nk_input_key(ctx, NK_KEY_TEXT_LINE_END, is_down);
  else if (keys_id == rb_intern("text_start"))        nk_input_key(ctx, NK_KEY_TEXT_START, is_down);
  else if (keys_id == rb_intern("text_end"))          nk_input_key(ctx, NK_KEY_TEXT_END, is_down);
  else if (keys_id == rb_intern("text_undo"))         nk_input_key(ctx, NK_KEY_TEXT_UNDO, is_down);
  else if (keys_id == rb_intern("text_redo"))         nk_input_key(ctx, NK_KEY_TEXT_REDO, is_down);
  else if (keys_id == rb_intern("text_select_all"))   nk_input_key(ctx, NK_KEY_TEXT_SELECT_ALL, is_down);
  else if (keys_id == rb_intern("text_word_left"))    nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, is_down);
  else if (keys_id == rb_intern("text_word_right"))   nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, is_down);

  /* Shortcuts: scrollbar */
  else if (keys_id == rb_intern("scroll_start")) nk_input_key(ctx, NK_KEY_SCROLL_START, is_down);
  else if (keys_id == rb_intern("scroll_end"))   nk_input_key(ctx, NK_KEY_SCROLL_END, is_down);
  else if (keys_id == rb_intern("scroll_down"))  nk_input_key(ctx, NK_KEY_SCROLL_DOWN, is_down);
  else if (keys_id == rb_intern("scroll_up"))    nk_input_key(ctx, NK_KEY_SCROLL_UP, is_down);
  else if (keys_id == rb_intern("max"))          nk_input_key(ctx, NK_KEY_MAX, is_down);
  else {
    VALUE s = rb_funcall(keys, rb_intern("inspect"), 0);
    rb_raise(rb_eArgError, "Invalid key symbol: %s", StringValueCStr(s));
  }

  return self;
}

VALUE nkrb_event_sink_button(VALUE self, VALUE buttons, VALUE rx, VALUE ry, VALUE down) {
  VALUE_TO_NK(rb_ivar_get(self, rb_intern("@context")), ctx);
  ID buttons_id = SYM2ID(buttons);
  int is_down = RTEST(down) ? 1 : 0;
  int x = FIX2INT(rx), y = FIX2INT(ry);

  if (buttons_id == rb_intern("left"))   nk_input_button(ctx, NK_BUTTON_LEFT,   x, y, is_down);
  if (buttons_id == rb_intern("middle")) nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, is_down);
  if (buttons_id == rb_intern("right"))  nk_input_button(ctx, NK_BUTTON_RIGHT,  x, y, is_down);
  if (buttons_id == rb_intern("double")) nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, is_down);
  if (buttons_id == rb_intern("max"))    nk_input_button(ctx, NK_BUTTON_MAX,    x, y, is_down);

  return self;
}

VALUE nkrb_event_sink_scroll(VALUE self, VALUE x, VALUE y) {
  VALUE_TO_NK(rb_ivar_get(self, rb_intern("@context")), ctx);
  struct nk_vec2 vec;
  vec.x = (float) NUM2DBL(x);
  vec.y = (float) NUM2DBL(y);
  nk_input_scroll(ctx, vec);
  return self;
}

VALUE nkrb_event_sink_char(VALUE self, VALUE chr) {
  VALUE_TO_NK(rb_ivar_get(self, rb_intern("@context")), ctx);
  int ch = FIX2INT(rb_funcall(chr, rb_intern("ord"), 0));
  nk_input_char(ctx, ch);
  return self;
}

VALUE nkrb_event_sink_glyph(VALUE self, VALUE chr) {
  VALUE_TO_NK(rb_ivar_get(self, rb_intern("@context")), ctx);
  nk_glyph glyph;
  int len = (int) RSTRING_LEN(chr);
  if (len < NK_UTF_SIZE) {
    if (len > 0) {
      int i;
      for (i = 0; i < len; i++) {
        int ch = FIX2INT(rb_funcall(rb_funcall(chr, rb_intern("[]"), 1, INT2FIX(i)),
                                    rb_intern("ord"), 0));
        nk_input_char(ctx, ch);
      }
    } else {
      rb_raise(rb_eArgError, "input is empty");
    }
  } else {
    memcpy(glyph, StringValuePtr(chr), NK_UTF_SIZE);
    nk_input_glyph(ctx, glyph);
  }
  return self;
}

// VALUE nkrb_event_sink_unicode(VALUE self, VALUE ch) {
//   
// }

VALUE nkrb_context_ui(VALUE self) {
  VALUE ui = rb_ivar_get(self, rb_intern("@ui_builder"));
  rb_need_block();
  rb_funcall_with_block(ui, rb_intern("instance_eval"), 0, NULL, rb_block_proc());
  return ui;
}

void nkrb_context_init(void) {
  cNuklearContext = rb_define_class_under(mNuklear, "Context", rb_cObject);
  rb_define_alloc_func(cNuklearContext, nkrb_context_alloc);
  rb_define_method(cNuklearContext, "user_data=", nkrb_context_set_user_data, 1);
  rb_define_method(cNuklearContext, "user_data", nkrb_context_get_user_data, 0);
  rb_define_method(cNuklearContext, "initialize", nkrb_context_initialize, 1);
  rb_define_method(cNuklearContext, "tick", nkrb_context_tick, 0);
  rb_define_method(cNuklearContext, "ui", nkrb_context_ui, 0);

  cNuklearContextEventSink = rb_define_class_under(cNuklearContext, "EventSink", rb_cObject);
  rb_define_method(cNuklearContextEventSink, "initialize", rb_nkrb_event_sink_initialize, 1);
  rb_define_method(cNuklearContextEventSink, "motion",  nkrb_event_sink_motion,  4);
  rb_define_method(cNuklearContextEventSink, "key",     nkrb_event_sink_key,     2);
  rb_define_method(cNuklearContextEventSink, "button",  nkrb_event_sink_button,  4);
  rb_define_method(cNuklearContextEventSink, "scroll",  nkrb_event_sink_scroll,  2);
  rb_define_method(cNuklearContextEventSink, "char",    nkrb_event_sink_char,    1);
  rb_define_method(cNuklearContextEventSink, "glyph",   nkrb_event_sink_glyph,   1);
  // rb_define_method(cNuklearContextEventSink, "unicode", nkrb_event_sink_unicode, 1);
}
