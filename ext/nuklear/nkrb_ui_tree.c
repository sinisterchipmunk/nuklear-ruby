#include "nkrb.h"

VALUE nkrb_ui_tree(VALUE self, VALUE rcontext, VALUE rtree_type, VALUE rimage, VALUE rtitle, VALUE rcurrent_state) {
  struct nk_context        *context = nkrb_context_get(rcontext);
  enum   nk_tree_type       tree_type = FIX2INT(rtree_type);
  enum   nk_collapse_states current_state = FIX2INT(rcurrent_state);
  const char *title = StringValueCStr(rtitle);
  int result;
  if (!NIL_P(rimage)) {
    struct nk_image image;
    ASSIGN_STYLE_AS_IMAGE(image, nkrb_style_item(rimage));
    result = nk_tree_state_image_push(context, tree_type, image, title, &current_state);
  } else {
    result = nk_tree_state_push(context, tree_type, title, &current_state);
  }

  if (rb_block_given_p()) {
    rb_yield(INT2FIX(current_state));
  }

  if (result)
    nk_tree_state_pop(context);

  return Qnil;
}

void nkrb_ui_tree_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_tree", nkrb_ui_tree, 5);
}
