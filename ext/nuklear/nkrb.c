#define NK_IMPLEMENTATION
#include "nkrb.h"

VALUE mNuklear = Qnil;

void Init_nuklear(void) {
  mNuklear = rb_define_module("Nuklear");

  rb_const_set(mNuklear, rb_intern("NK_WINDOW_BORDER"),             INT2FIX(NK_WINDOW_BORDER));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_MOVABLE"),            INT2FIX(NK_WINDOW_MOVABLE));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_SCALABLE"),           INT2FIX(NK_WINDOW_SCALABLE));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_CLOSABLE"),           INT2FIX(NK_WINDOW_CLOSABLE));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_MINIMIZABLE"),        INT2FIX(NK_WINDOW_MINIMIZABLE));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_NO_SCROLLBAR"),       INT2FIX(NK_WINDOW_NO_SCROLLBAR));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_TITLE"),              INT2FIX(NK_WINDOW_TITLE));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_SCROLL_AUTO_HIDE"),   INT2FIX(NK_WINDOW_SCROLL_AUTO_HIDE));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_BACKGROUND"),         INT2FIX(NK_WINDOW_BACKGROUND));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_SCALE_LEFT"),         INT2FIX(NK_WINDOW_SCALE_LEFT));
  rb_const_set(mNuklear, rb_intern("NK_WINDOW_NO_INPUT"),           INT2FIX(NK_WINDOW_NO_INPUT));

  rb_const_set(mNuklear, rb_intern("NK_EDIT_DEFAULT"),              INT2FIX(NK_EDIT_DEFAULT));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_READ_ONLY"),            INT2FIX(NK_EDIT_READ_ONLY));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_AUTO_SELECT"),          INT2FIX(NK_EDIT_AUTO_SELECT));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_SIG_ENTER"),            INT2FIX(NK_EDIT_SIG_ENTER));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_ALLOW_TAB"),            INT2FIX(NK_EDIT_ALLOW_TAB));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_NO_CURSOR"),            INT2FIX(NK_EDIT_NO_CURSOR));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_SELECTABLE"),           INT2FIX(NK_EDIT_SELECTABLE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_CLIPBOARD"),            INT2FIX(NK_EDIT_CLIPBOARD));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_CTRL_ENTER_NEWLINE"),   INT2FIX(NK_EDIT_CTRL_ENTER_NEWLINE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_NO_HORIZONTAL_SCROLL"), INT2FIX(NK_EDIT_NO_HORIZONTAL_SCROLL));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_ALWAYS_INSERT_MODE"),   INT2FIX(NK_EDIT_ALWAYS_INSERT_MODE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_MULTILINE"),            INT2FIX(NK_EDIT_MULTILINE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_GOTO_END_ON_ACTIVATE"), INT2FIX(NK_EDIT_GOTO_END_ON_ACTIVATE));

  rb_const_set(mNuklear, rb_intern("NK_EDIT_SIMPLE"),               INT2FIX(NK_EDIT_SIMPLE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_FIELD"),                INT2FIX(NK_EDIT_FIELD));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_BOX"),                  INT2FIX(NK_EDIT_BOX));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_EDITOR"),               INT2FIX(NK_EDIT_EDITOR));

  rb_const_set(mNuklear, rb_intern("NK_EDIT_ACTIVE"),               INT2FIX(NK_EDIT_ACTIVE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_INACTIVE"),             INT2FIX(NK_EDIT_INACTIVE));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_ACTIVATED"),            INT2FIX(NK_EDIT_ACTIVATED));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_DEACTIVATED"),          INT2FIX(NK_EDIT_DEACTIVATED));
  rb_const_set(mNuklear, rb_intern("NK_EDIT_COMMITED"),             INT2FIX(NK_EDIT_COMMITED));

  rb_const_set(mNuklear, rb_intern("NK_STYLE_ITEM_COLOR"),          INT2FIX(NK_STYLE_ITEM_COLOR));
  rb_const_set(mNuklear, rb_intern("NK_STYLE_ITEM_IMAGE"),          INT2FIX(NK_STYLE_ITEM_IMAGE));

  rb_const_set(mNuklear, rb_intern("NK_MINIMIZED"),                 INT2FIX(NK_MINIMIZED));
  rb_const_set(mNuklear, rb_intern("NK_MAXIMIZED"),                 INT2FIX(NK_MAXIMIZED));

  rb_const_set(mNuklear, rb_intern("NK_TREE_NODE"),                 INT2FIX(NK_TREE_NODE));
  rb_const_set(mNuklear, rb_intern("NK_TREE_TAB"),                  INT2FIX(NK_TREE_TAB));

  rb_const_set(mNuklear, rb_intern("NK_DYNAMIC"),                   INT2FIX(NK_DYNAMIC));
  rb_const_set(mNuklear, rb_intern("NK_STATIC"),                    INT2FIX(NK_STATIC));

  rb_const_set(mNuklear, rb_intern("NK_POPUP_DYNAMIC"),             INT2FIX(NK_POPUP_DYNAMIC));
  rb_const_set(mNuklear, rb_intern("NK_POPUP_STATIC"),              INT2FIX(NK_POPUP_STATIC));

  rb_const_set(mNuklear, rb_intern("NK_TEXT_ALIGN_LEFT"),           INT2FIX(NK_TEXT_ALIGN_LEFT));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_ALIGN_CENTERED"),       INT2FIX(NK_TEXT_ALIGN_CENTERED));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_ALIGN_RIGHT"),          INT2FIX(NK_TEXT_ALIGN_RIGHT));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_ALIGN_TOP"),            INT2FIX(NK_TEXT_ALIGN_TOP));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_ALIGN_MIDDLE"),         INT2FIX(NK_TEXT_ALIGN_MIDDLE));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_ALIGN_BOTTOM"),         INT2FIX(NK_TEXT_ALIGN_BOTTOM));

  rb_const_set(mNuklear, rb_intern("NK_TEXT_LEFT"),                 INT2FIX(NK_TEXT_LEFT));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_CENTERED"),             INT2FIX(NK_TEXT_CENTERED));
  rb_const_set(mNuklear, rb_intern("NK_TEXT_RIGHT"),                INT2FIX(NK_TEXT_RIGHT));

  nkrb_context_init();
  nkrb_font_init();
  nkrb_buffer_init();
  nkrb_renderer_init();
  nkrb_style_init();
  nkrb_ui_builder_init();
  nkrb_ui_init();
}
