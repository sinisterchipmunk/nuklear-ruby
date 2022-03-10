#include "nkrb.h"

VALUE cNuklearBuffer = Qnil;

void nkrb_buffer_dynamic_free(struct nk_buffer *buf) {
  // need to let nk free the buffer contents, then free the buffer itself.
  nk_buffer_free(buf);
  free(buf);
}

void nkrb_buffer_managed_free(struct nk_buffer *buf) {
  // the buffer memory is managed elsewhere, and will be freed elsewhere.
  // We only need to free the buffer itself.
  free(buf);
}

void nkrb_buffer_fixed_free(struct nk_buffer *buf) {
  // need to free the memory we allocated ourselves, then free the buffer
  // itself.
  free(nk_buffer_memory(buf));
  free(buf);
}

VALUE nkrb_buffer_new(int argc, VALUE* argv, VALUE module) {
  VALUE rsize = Qnil;
  VALUE rptr = Qnil;
  rb_scan_args(argc, argv, "02", &rsize, &rptr); // optional rsize, rptr
  void (*freefn)(struct nk_buffer *buf) = NULL;

  struct nk_buffer *buf = malloc(sizeof(struct nk_buffer));
  nk_size size = 0; // if size is 0, the buffer will be dynamic.
  if (!NIL_P(rsize)) size = (nk_size) NUM2INT(rsize);

  if (size) {
    void *mem = NULL;
    if (!NIL_P(rptr)) {
      // we were passed a pointer; we will treat it as a 64 bit unsigned in
      // ruby, and a void * in c. We will not zero the user-provided buffer.
      mem = (void *) NUM2ULL(rptr);
      freefn = nkrb_buffer_managed_free;
    } else {
      // we were not passed a pointer; we will allocate a buffer of the
      // requested size and use that. We will zero the uninitialized memory.
      mem = malloc(size);
      memset(mem, 0, size);
      freefn = nkrb_buffer_fixed_free;
    }
    nk_buffer_init_fixed(buf, mem, size);
  } else {
    nk_buffer_init_default(buf);
    freefn = nkrb_buffer_dynamic_free;
  }
  VALUE result = Data_Wrap_Struct(cNuklearBuffer, NULL, freefn, buf);
  return result;
}

VALUE nkrb_buffer_size(VALUE self) {
  struct nk_buffer *buf = NULL;
  Data_Get_Struct(self, struct nk_buffer, buf);
  return ULONG2NUM(nk_buffer_total(buf));
}

VALUE nkrb_buffer_to_ptr(VALUE self) {
  struct nk_buffer *buf = NULL;
  Data_Get_Struct(self, struct nk_buffer, buf);
  VALUE cFiddle = rb_const_get(rb_cObject, rb_intern("Fiddle"));
  VALUE cPointer = rb_const_get(cFiddle, rb_intern("Pointer"));
  VALUE fiddleptr = rb_funcall(cPointer, rb_intern("new"), 2,
                               ULONG2NUM((unsigned long) nk_buffer_memory(buf)),
                               nkrb_buffer_size(self));
  return fiddleptr;
}

void nkrb_buffer_init(void) {
  cNuklearBuffer = rb_define_class_under(mNuklear, "Buffer", rb_cObject);

  rb_define_module_function(cNuklearBuffer, "new", nkrb_buffer_new, -1);
  rb_define_method(cNuklearBuffer, "size", nkrb_buffer_size, 0);
  rb_define_method(cNuklearBuffer, "to_ptr", nkrb_buffer_to_ptr, 0);
}
