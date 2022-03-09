#include "nkrb.h"

VALUE nkrb_ui_edit_string(VALUE self, VALUE context, VALUE rflags, VALUE rbuffer, VALUE max_length, VALUE rfilter) {
  Check_Type(rbuffer, T_STRING);
  rfilter = rb_funcall(rfilter, rb_intern("to_s"), 0);
  int buffer_size = FIX2INT(max_length) + 1;
  char *buffer = malloc(buffer_size);
  memset(buffer, 0, buffer_size);
  char *ptr = RSTRING_PTR(rbuffer);
  int rbuffer_len = (int) RSTRING_LEN(rbuffer);
  if (rbuffer_len > buffer_size - 1) rbuffer_len = buffer_size - 1;
  memcpy(buffer, ptr, rbuffer_len);
  nk_plugin_filter filter = nk_filter_default;
  if (!strcmp(StringValueCStr(rfilter), "ascii"))   filter = nk_filter_ascii;
  if (!strcmp(StringValueCStr(rfilter), "float"))   filter = nk_filter_float;
  if (!strcmp(StringValueCStr(rfilter), "decimal")) filter = nk_filter_decimal;
  if (!strcmp(StringValueCStr(rfilter), "hex"))     filter = nk_filter_hex;
  if (!strcmp(StringValueCStr(rfilter), "octal"))   filter = nk_filter_oct;
  if (!strcmp(StringValueCStr(rfilter), "binary"))  filter = nk_filter_binary;
  int start_len = rbuffer_len;
  nk_flags result = nk_edit_string(nkrb_context_get(context), FIX2INT(rflags), buffer, &rbuffer_len, buffer_size - 1, filter);
  if (rbuffer_len != start_len || memcmp(buffer, ptr, rbuffer_len)) {
    rb_funcall(rbuffer, rb_intern("clear"), 0);
    rb_str_cat(rbuffer, buffer, rbuffer_len);
  }
  free(buffer);
  return INT2FIX(result);
}

VALUE nkrb_ui_edit_focus(VALUE self, VALUE context, VALUE rflags) {
  nk_edit_focus(nkrb_context_get(context), FIX2INT(rflags));
  return Qnil;
}

void nkrb_ui_edit_string_init(void) {
  rb_define_method(mNuklearUIContainer, "ui_edit_string", nkrb_ui_edit_string, 5);
  rb_define_method(mNuklearUIContainer, "ui_edit_focus",  nkrb_ui_edit_focus, 2);
}
