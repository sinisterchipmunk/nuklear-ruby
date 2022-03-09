#ifndef NUKLEAR_RUBY_H
#define NUKLEAR_RUBY_H

#include "nuklear.h"
#include <ruby.h>

#define ASSIGN_STYLE_AS_IMAGE(left, style) \
  if (style->type == NK_STYLE_ITEM_IMAGE)  \
    left = style->data.image;              \
  else                                     \
    rb_raise(rb_eArgError, "expected an image here");

#define ASSIGN_STYLE_AS_COLOR(left, style) \
  if (style->type == NK_STYLE_ITEM_COLOR)  \
    left = style->data.color;              \
  else                                     \
    rb_raise(rb_eArgError, "expected a color here");


#define SET_RSYMBOL(symbol, rsymbol) \
  if (!NIL_P(rsymbol)) {                                                                        \
    if      (SYM2ID(rsymbol) == rb_intern("x"))              symbol = NK_SYMBOL_X;              \
    else if (SYM2ID(rsymbol) == rb_intern("underscore"))     symbol = NK_SYMBOL_UNDERSCORE;     \
    else if (SYM2ID(rsymbol) == rb_intern("circle_solid"))   symbol = NK_SYMBOL_CIRCLE_SOLID;   \
    else if (SYM2ID(rsymbol) == rb_intern("circle_outline")) symbol = NK_SYMBOL_CIRCLE_OUTLINE; \
    else if (SYM2ID(rsymbol) == rb_intern("rect_solid"))     symbol = NK_SYMBOL_RECT_SOLID;     \
    else if (SYM2ID(rsymbol) == rb_intern("rect_outline"))   symbol = NK_SYMBOL_RECT_OUTLINE;   \
    else if (SYM2ID(rsymbol) == rb_intern("triangle_up"))    symbol = NK_SYMBOL_TRIANGLE_UP;    \
    else if (SYM2ID(rsymbol) == rb_intern("triangle_down"))  symbol = NK_SYMBOL_TRIANGLE_DOWN;  \
    else if (SYM2ID(rsymbol) == rb_intern("triangle_left"))  symbol = NK_SYMBOL_TRIANGLE_LEFT;  \
    else if (SYM2ID(rsymbol) == rb_intern("triangle_right")) symbol = NK_SYMBOL_TRIANGLE_RIGHT; \
    else if (SYM2ID(rsymbol) == rb_intern("plus"))           symbol = NK_SYMBOL_PLUS;           \
    else if (SYM2ID(rsymbol) == rb_intern("minus"))          symbol = NK_SYMBOL_MINUS;          \
    else if (SYM2ID(rsymbol) == rb_intern("max"))            symbol = NK_SYMBOL_MAX;            \
    else rb_raise(rb_eArgError, "Symbol must be :x, :underscore, :circle_solid, :circle_outline"\
                                ", :rect_solid, :rect_outline, :triangle_up, :triangle_down, "  \
                                ":triangle_left, :triangle_right, :plus, :minus or :max.");     \
  }


void nkrb_context_init(void);
void nkrb_font_init(void);
void nkrb_buffer_init(void);
void nkrb_renderer_init(void);
void nkrb_ui_builder_init(void);
void nkrb_ui_init(void);
void nkrb_ui_button_init(void);
void nkrb_ui_checkbox_init(void);
void nkrb_ui_color_picker_init(void);
void nkrb_ui_combo_init(void);
void nkrb_ui_group_init(void);
void nkrb_ui_label_init(void);
void nkrb_ui_layout_init(void);
void nkrb_ui_edit_string_init(void);
void nkrb_ui_menu_init(void);
void nkrb_ui_menubar_init(void);
void nkrb_ui_menu_item_init(void);
void nkrb_ui_popup_init(void);
void nkrb_ui_progress_init(void);
void nkrb_ui_property_init(void);
void nkrb_ui_slider_init(void);
void nkrb_ui_tree_init(void);
void nkrb_ui_widget_init(void);
void nkrb_ui_window_init(void);
void nkrb_ui_container_init(void);
void nkrb_style_init(void);
void nkrb_style_color_init(void);
void nkrb_style_image_init(void);
struct nk_user_font *nkrb_font_to_nk(VALUE self);
struct nk_context *nkrb_context_get(VALUE context);
struct nk_style_item *nkrb_style_item(VALUE value);
void nkrb_style_item_free(struct nk_style_item *item);
VALUE nkrb_font_get_null(VALUE value);
void instance_eval_block(VALUE self);
void assign_button_style_from_ruby(struct nk_style_button *style, VALUE rstyle);

extern VALUE mNuklear;
extern VALUE cNuklearContext;
extern VALUE cNuklearFont;
extern VALUE cNuklearBuffer;
extern VALUE cNuklearUIBuilder;
extern VALUE mNuklearUIContainer;
extern VALUE mNuklearUI;
extern VALUE cNuklearUIWidget;
extern VALUE mNuklearStyle;
extern VALUE cNuklearStyleColor;
extern VALUE cNuklearStyleImage;

#endif // NUKLEAR_RUBY_H
