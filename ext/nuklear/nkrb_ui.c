#include "nkrb.h"

VALUE mNuklearUI = Qnil;

void instance_eval_block(VALUE self) {
  rb_need_block();
  rb_funcall_with_block(self, rb_intern("instance_eval"), 0, NULL, rb_block_proc());
}

void nkrb_ui_init(void) {
  mNuklearUI = rb_define_module_under(mNuklear, "UI");
  nkrb_ui_container_init();

  nkrb_ui_button_init();
  nkrb_ui_checkbox_init();
  nkrb_ui_color_picker_init();
  nkrb_ui_combo_init();
  nkrb_ui_edit_string_init();
  nkrb_ui_group_init();
  nkrb_ui_label_init();
  nkrb_ui_layout_init();
  nkrb_ui_menu_init();
  nkrb_ui_menu_item_init();
  nkrb_ui_menubar_init();
  nkrb_ui_popup_init();
  nkrb_ui_progress_init();
  nkrb_ui_property_init();
  nkrb_ui_slider_init();
  nkrb_ui_tree_init();
  nkrb_ui_widget_init();
  nkrb_ui_window_init();
}
