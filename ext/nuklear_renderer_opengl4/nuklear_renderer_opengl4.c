/* Heavily adapted from https://github.com/vurtun/nuklear/blob/master/demo/sdl_opengl3/nuklear_sdl_gl3.h */

#define NK_IMPLEMENTATION
#include "../nuklear/nuklear.h"
#include "glad.h"
#include <ruby.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

VALUE mNuklear = Qnil;
VALUE cNuklearRenderer = Qnil;
VALUE cNuklearRendererOpenGL4 = Qnil;
VALUE cNuklearBuffer = Qnil;

#ifdef __APPLE__
  #define NK_SHADER_VERSION "#version 150\n"
#else
  #define NK_SHADER_VERSION "#version 300 es\n"
#endif

static const GLchar *vertex_shader =
        NK_SHADER_VERSION
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 TexCoord;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main() {\n"
        "   Frag_UV = TexCoord;\n"
        "   Frag_Color = Color;\n"
        "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
        "}\n";

static const GLchar *fragment_shader =
    NK_SHADER_VERSION
    "precision mediump float;\n"
    "uniform sampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "void main(){\n"
    "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    "}\n";

struct vertex {
  float position[2];
  float uv[2];
  nk_byte col[4];
};

void nuklear_command_buffer_free(struct nk_buffer *buf) {
  nk_buffer_free(buf);
}

VALUE renderer_initialize(VALUE self, VALUE context) {
  if (!gladLoadGL())
    rb_raise(rb_eStandardError, "Failed to init GLAD");
  // printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

  rb_call_super(1, &context);

  int status;
  int prog = glCreateProgram();
  int vert_shdr = glCreateShader(GL_VERTEX_SHADER);
  int frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_shdr, 1, &vertex_shader, 0);
  glShaderSource(frag_shdr, 1, &fragment_shader, 0);
  glCompileShader(vert_shdr);
  glCompileShader(frag_shdr);
  glGetShaderiv(vert_shdr, GL_COMPILE_STATUS, &status);
  assert(status == GL_TRUE);
  glGetShaderiv(frag_shdr, GL_COMPILE_STATUS, &status);
  assert(status == GL_TRUE);
  glAttachShader(prog, vert_shdr);
  glAttachShader(prog, frag_shdr);
  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  assert(status == GL_TRUE);

  int uniform_tex  = glGetUniformLocation(prog, "Texture");
  int uniform_proj = glGetUniformLocation(prog, "ProjMtx");
  int attrib_pos   = glGetAttribLocation(prog, "Position");
  int attrib_uv    = glGetAttribLocation(prog, "TexCoord");
  int attrib_col   = glGetAttribLocation(prog, "Color");

  /* buffer setup */
  GLsizei vs = sizeof(struct vertex);
  size_t vp = offsetof(struct vertex, position);
  size_t vt = offsetof(struct vertex, uv);
  size_t vc = offsetof(struct vertex, col);

  GLuint vbo, ebo, vao;

  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glEnableVertexAttribArray((GLuint)attrib_pos);
  glEnableVertexAttribArray((GLuint)attrib_uv);
  glEnableVertexAttribArray((GLuint)attrib_col);

  glVertexAttribPointer((GLuint)attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
  glVertexAttribPointer((GLuint)attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
  glVertexAttribPointer((GLuint)attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  rb_ivar_set(self, rb_intern("@program"),      INT2FIX(prog));
  rb_ivar_set(self, rb_intern("@uniform_tex"),  INT2FIX(uniform_tex));
  rb_ivar_set(self, rb_intern("@uniform_proj"), INT2FIX(uniform_proj));
  rb_ivar_set(self, rb_intern("@vao"),          INT2FIX(vao));
  rb_ivar_set(self, rb_intern("@vbo"),          INT2FIX(vbo));
  rb_ivar_set(self, rb_intern("@ebo"),          INT2FIX(ebo));

  return self;
}

VALUE renderer_render_gl(VALUE self) {
  struct nk_context *ctx;
  struct nk_buffer *cmds;
  VALUE window_size = rb_funcall(self, rb_intern("window_size"), 0);
  VALUE drawable_size = rb_funcall(self, rb_intern("drawable_size"), 0);
  VALUE window_width = rb_ary_entry(window_size, 0);
  VALUE window_height = rb_ary_entry(window_size, 1);
  VALUE drawable_width = rb_ary_entry(drawable_size, 0);
  VALUE drawable_height = rb_ary_entry(drawable_size, 1);
  int program = FIX2INT(rb_ivar_get(self, rb_intern("@program")));
  int uniform_tex = FIX2INT(rb_ivar_get(self, rb_intern("@uniform_tex")));
  int uniform_proj = FIX2INT(rb_ivar_get(self, rb_intern("@uniform_proj")));
  int width = FIX2INT(window_width),
      height = FIX2INT(window_height);
  int display_width = FIX2INT(drawable_width),
      display_height = FIX2INT(drawable_height);
  struct nk_draw_null_texture *null_tex;
  struct nk_vec2 scale;
  VALUE context = rb_ivar_get(self, rb_intern("@context"));
  Data_Get_Struct(rb_ivar_get(context, rb_intern("@null")), struct nk_draw_null_texture, null_tex);
  Data_Get_Struct(context, struct nk_context, ctx);
  Data_Get_Struct(rb_ivar_get(self, rb_intern("@commands")), struct nk_buffer, cmds);

  GLfloat ortho[4][4] = {
      {2.0f, 0.0f, 0.0f, 0.0f},
      {0.0f,-2.0f, 0.0f, 0.0f},
      {0.0f, 0.0f,-1.0f, 0.0f},
      {-1.0f,1.0f, 0.0f, 1.0f},
  };
  ortho[0][0] /= (GLfloat)width;
  ortho[1][1] /= (GLfloat)height;

  scale.x = (float)display_width/(float)width;
  scale.y = (float)display_height/(float)height;

  /* setup global state */
  glViewport(0,0,display_width,display_height);
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glActiveTexture(GL_TEXTURE0);

  /* setup program */
  glUseProgram(program);
  glUniform1i(uniform_tex, 0);
  glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, &ortho[0][0]);

  rb_call_super(0, NULL);

  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDisable(GL_SCISSOR_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  return self;
}

VALUE renderer_nk_convert_gl(VALUE self) {
  VALUE result = rb_call_super(0, NULL);

  struct nk_buffer *vertices = NULL;
  struct nk_buffer *indices  = NULL;
  Data_Get_Struct(rb_funcall(self, rb_intern("vertices"), 0),       struct nk_buffer, vertices);
  Data_Get_Struct(rb_funcall(self, rb_intern("vertex_indices"), 0), struct nk_buffer, indices);

  int vao = FIX2INT(rb_ivar_get(self, rb_intern("@vao")));
  int vbo = FIX2INT(rb_ivar_get(self, rb_intern("@vbo")));
  int ebo = FIX2INT(rb_ivar_get(self, rb_intern("@ebo")));
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ARRAY_BUFFER,
               MAX_VERTEX_MEMORY,
               nk_buffer_memory_const(vertices),
               GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               MAX_ELEMENT_MEMORY,
               nk_buffer_memory_const(indices),
               GL_STREAM_DRAW);
  return result;
}

VALUE renderer_draw_gl(VALUE self, VALUE cmd) {
  VALUE window_size = rb_funcall(self, rb_intern("window_size"), 0);
  VALUE drawable_size = rb_funcall(self, rb_intern("drawable_size"), 0);
  VALUE window_width = rb_ary_entry(window_size, 0);
  VALUE window_height = rb_ary_entry(window_size, 1);
  VALUE drawable_width = rb_ary_entry(drawable_size, 0);
  VALUE drawable_height = rb_ary_entry(drawable_size, 1);
  int width = FIX2INT(window_width),
      height = FIX2INT(window_height);
  int display_width = FIX2INT(drawable_width),
      display_height = FIX2INT(drawable_height);
  struct nk_vec2 scale;
  scale.x = (float)display_width/(float)width;
  scale.y = (float)display_height/(float)height;

  VALUE clip_rect = rb_hash_aref(cmd, ID2SYM(rb_intern("clip_rect")));
  float x = (float) NUM2DBL(RARRAY_AREF(clip_rect, 0));
  float y = (float) NUM2DBL(RARRAY_AREF(clip_rect, 1));
  float w = (float) NUM2DBL(RARRAY_AREF(clip_rect, 2));
  float h = (float) NUM2DBL(RARRAY_AREF(clip_rect, 3));
  glBindTexture(GL_TEXTURE_2D, (GLuint) NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("texture_handle")))));
  glScissor((GLint)(x * scale.x),
            (GLint)((height - (GLint)(y + h)) * scale.y),
            (GLint)(w * scale.x),
            (GLint)(h * scale.y));
  glDrawElements(GL_TRIANGLES, (GLsizei)NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("element_count")))),
                 GL_UNSIGNED_SHORT, (void *) NUM2UINT(rb_hash_aref(cmd, ID2SYM(rb_intern("offset")))));
  return self;
}

void Init_nuklear_renderer_opengl4(void) {
  mNuklear = rb_define_module("Nuklear");
  cNuklearRenderer = rb_define_class_under(mNuklear, "Renderer", rb_cObject);
  cNuklearRendererOpenGL4 = rb_define_class_under(cNuklearRenderer, "OpenGL4", cNuklearRenderer);
  cNuklearBuffer = rb_const_get(mNuklear, rb_intern("Buffer"));

  rb_define_method(cNuklearRendererOpenGL4, "initialize", renderer_initialize, 1);
  rb_define_method(cNuklearRendererOpenGL4, "nk_convert", renderer_nk_convert_gl, 0);
  rb_define_method(cNuklearRendererOpenGL4, "draw",       renderer_draw_gl, 1);
  rb_define_method(cNuklearRendererOpenGL4, "render",     renderer_render_gl, 0);
}
