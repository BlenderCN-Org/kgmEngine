// kgmOGL.cpp: implementation of the kgmOGL class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmOGL.h"
#include "kgmOGLExt.h"
#include "kgmOGLWindow.h"
#include "../kgmBase/kgmLog.h"


#ifdef WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#ifdef DEBUG
#ifndef ANDROID
//#include <GL/glu.h>
#endif
#endif

GLu32          m_rendertarget = 0;
GLint*         g_compressed_format = null;
GLint          g_num_compressed_format = 0;

GLhandle       g_shader = null;


#ifdef ANDROID
EGLDisplay  m_display       = EGL_NO_DISPLAY;
EGLSurface  m_renderSurface = EGL_NO_SURFACE;
EGLContext  m_context       = EGL_NO_CONTEXT;
#endif

kgmOGL::kgmOGL(kgmOGLWindow *wnd)
{
  m_is_shader      = 0;
  m_is_framebuffer = 0;

  if(!wnd)
    return;

  this->m_wnd = wnd;

  GLubyte* oglVersion = (GLubyte*)glGetString(GL_VERSION);

#ifdef DEBUG
  kgm_log() << "OpenGL Version: " << (char*)oglVersion << "\n";
#else
  (void)oglVersion;
#endif

  glInitExt();
  glEnable(GL_ACTIVE_TEXTURE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

#ifdef GL_SMOOTH
  glShadeModel(GL_SMOOTH);
#endif

#ifdef GL_PROJECTION
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#endif

  const GLubyte* ext = glGetString(GL_EXTENSIONS);
  //const GLubyte* ext = glGetString(GL_SHADING_LANGUAGE_VERSION);
  //FILE* f = fopen("/tmp/glext", "w");
  //fprintf(f, "%s", ext);
  //fclose(f);

  if(strstr((char*)ext, "GL_ARB_shader_objects"))
  {
    m_is_shader = 1;
  }

  if(strstr((char*)ext, "GL_ARB_framebuffer_object"))
  {
    m_is_framebuffer = 1;
  }

#ifdef GLES_2
  m_is_shader = 1;
  m_is_framebuffer = 1;
#endif

#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
  glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &g_num_compressed_format);

  g_compressed_format = (GLint*)malloc(g_num_compressed_format * sizeof(GLint));

  glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, g_compressed_format);

  for(int i = 0; i < g_num_compressed_format; i++)
  {
    int k = g_compressed_format[i];
    int a = 0;
  }
#endif

#ifdef WIN32
  //if(wglSwapIntervalEXT) wglSwapIntervalEXT(0);
#endif

#ifdef LINUX
  //if(glxSwapIntervalSGI) glxSwapIntervalSGI(0);
#endif

  //init local values
  glEnable(GL_TEXTURE_2D);

  m_renderbuffer = 0;
  m_lights       = 0;

  m_min_filter = GL_LINEAR;
  m_mag_filter = GL_LINEAR;
}

kgmOGL::~kgmOGL()
{
  m_rendertarget = 0;

#ifdef GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB
  if(g_compressed_format)
    free(g_compressed_format);
#endif

#ifdef DEBUG
  kgm_log() << "kgmOGL::~kgmOGL.\n";
#endif
}

void kgmOGL::gcSet(u32 param, void* value)
{
  switch(param)
  {
  case gcpar_blending:
    if(value)
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
    break;
  case gcpar_culling:
    if(value)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);
    break;
  case gcpar_textures:
    if(value)
      glEnable(GL_TEXTURE_2D);
    else
      glDisable(GL_TEXTURE_2D);
    break;
  case gcpar_depth:
    if(value)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
    break;
#ifdef GL_ALPHA_TEST
  case gcpar_alpha:
    if(value)
      glEnable(GL_ALPHA_TEST);
    else
      glDisable(GL_ALPHA_TEST);
    break;
#endif
  case gcpar_stencil:
    if(value)
      glEnable(GL_STENCIL_TEST);
    else
      glDisable(GL_STENCIL_TEST);
    break;
  case gcpar_colormask:
  {
    char* c = (char*)&value;
    GLboolean r = (c[0])?(GL_TRUE):(GL_FALSE),
        g = (c[1])?(GL_TRUE):(GL_FALSE),
        b = (c[2])?(GL_TRUE):(GL_FALSE),
        a = (c[3])?(GL_TRUE):(GL_FALSE);
    glColorMask(r, g, b, a);
  }
    break;
  case gctex_fltmag:
      m_mag_filter = gl_enum((u32)(size_t)value);
    break;
  case gctex_fltmin:
      m_min_filter = gl_enum((u32)(size_t)value);
    break;
  }
}

void kgmOGL::gcGet(u32 param, void* value)
{
  switch(param)
  {
  case gcsup_shaders:
    *((u32*)value) = m_is_shader;
    break;
  case gcsup_fbuffers:
    *((u32*)value) = m_is_framebuffer;
    break;
  case gc_rdev:
    //*((const char*)value) = "opengl";
    break;
  }
}

void kgmOGL::gcClear(u32 flag, u32 col, float depth, u32 sten)
{
  GLu32 cl = 0;

  if(flag & gcflag_color)
  {
    cl |= GL_COLOR_BUFFER_BIT;
    float inv = 1.0f / 255.0f;
    float a = inv * ((col & 0xff000000) >> 24),
        r = inv * ((col & 0x00ff0000) >> 16),
        g = inv * ((col & 0x0000ff00) >> 8),
        b = inv *  (col & 0x000000ff);

    glClearColor(r, g, b, a);
  }

  if(flag & gcflag_depth)
  {
    cl |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(depth);
  }

  if(flag & gcflag_stencil)
  {
    cl |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(sten);
  }

  glClear(cl);
}

void kgmOGL::gcBegin()
{
}

void kgmOGL::gcEnd()
{
}

void kgmOGL::gcRender()
{
#ifdef WIN32

  SwapBuffers(m_wnd->m_hdc);

#elif defined(ANDROID)

  eglSwapBuffers(m_wnd->display, m_wnd->surface);

#else

  glXSwapBuffers(m_wnd->m_dpy, m_wnd->m_wnd);

#endif
}

void kgmOGL::gcSetTarget(void* t)
{
#ifdef GL_FRAMEBUFFER
  if(!t || !((Texture*)t)->buffer)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, ((Texture*)t)->buffer);
  GLenum err = glGetError();

  if(err != GL_NO_ERROR)
  {
  }

  GLint ipar = 0;
  glEnable(GL_DEPTH_TEST);
  glDepthMask(true);
  glDepthFunc(GL_LEQUAL);
  glGetIntegerv(GL_DEPTH_BITS, &ipar);

  if(ipar == 0)
  {
  }
#endif
  //  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  //  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
  //  glDrawBuffer(GL_BACK);
  //  glReadBuffer(GL_BACK);
}

void kgmOGL::gcSetMatrix(u32 mode, float* mtx)
{
#ifdef GL_PROJECTION
  switch(mode)
  {
  case gcmtx_proj:
    glMatrixMode(GL_PROJECTION);
    break;
  case gcmtx_view:
    glMatrixMode(GL_MODELVIEW);
    break;
  case gcmtx_text:
    glMatrixMode(GL_TEXTURE);
    break;
  }

  glLoadMatrixf(mtx);
#endif
}

void kgmOGL::gcGetMatrix(u32 mode, float* mtx)
{
#ifdef GL_PROJECTION
  switch(mode)
  {
  case gcmtx_proj:
    glGetFloatv(GL_PROJECTION_MATRIX, mtx);
    break;
  case gcmtx_view:
    glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
    break;
  case gcmtx_text:
    glGetFloatv(GL_TEXTURE_MATRIX, mtx);
    break;
  }
#endif
}

void kgmOGL::gcSetViewport(int x, int y, int w, int h, float n, float f)
{
  glViewport(x, y, w, h);
}

//Light
void kgmOGL::gcSetLight(int i, float* pos, float range, float* col, float* dir, float angle)
{
#ifndef GLES_2
  if(i > GL_MAX_LIGHTS)
    return;

  if(i < 0)
  {
    int l = (int)fabs(i) - 1;

    if(glIsEnabled(GL_LIGHT0 + l))
    {
      glDisable(GL_LIGHT0 + l);
      m_lights--;
    }

    if(m_lights < 1)
      glDisable(GL_LIGHTING);
    return;
  }

  if(!glIsEnabled(GL_LIGHT0 + i))
  {
    glEnable(GL_LIGHT0 + i);
    m_lights++;

    if(!glIsEnabled(GL_LIGHTING))
      glEnable(GL_LIGHTING);
  }

  glLightfv(GL_LIGHT0 + i, GL_POSITION, (float*)pos);
  glLightfv(GL_LIGHT0 + i, GL_AMBIENT,  (float*)col);
  glLightfv(GL_LIGHT0 + i, GL_DIFFUSE,  (float*)col);
  glLightf(GL_LIGHT0  + i, GL_LINEAR_ATTENUATION, 1.0f - range);

  vec3 v(dir[0], dir[1], dir[2]);

  if(v.length() > 0)
  {
    glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION,  (float*)dir);
    glLightfv(GL_LIGHT0 + i, GL_SPOT_CUTOFF,  (float*)&angle);
  }
#endif
}

//FOG
/*
void kgmOGL::gcFog(kgmFog* fog){
 if(!fog){
  glDisable(GL_FOG);
  return;
 }

 glFogi(GL_FOG_MODE, (fog->m_mode)?(GL_EXP):(GL_LINEAR));
 glFogf(GL_FOG_DENSITY, fog->m_density);
 glFogf(GL_FOG_START, fog->m_start);
 glFogf(GL_FOG_END, fog->m_end);
 glFogfv(GL_FOG_COLOR, (float*)&fog->m_color);
 glEnable(GL_FOG);
}
*/

//BLEND
void  kgmOGL::gcBlend(bool en, u32 fnsrc, u32 fndst)
{
  if(!en)
  {
    glDisable(GL_BLEND);

    return;
  }

  glBlendFunc(gl_enum(fnsrc), gl_enum(fndst));
  glEnable(GL_BLEND);
}

//ALPHA
void  kgmOGL::gcAlpha(bool en, u32 fn, float ref)
{
#ifdef GL_ALPHA_TEST
  if(!en)
  {
    glDisable(GL_ALPHA_TEST);

    return;
  }
  glAlphaFunc(gl_enum(fn), ref);
  glEnable(GL_ALPHA_TEST);
#endif
}

//CULL
void kgmOGL::gcCull(u32 mode)
{
  if(!mode)
  {
    glDisable(GL_CULL_FACE);

    return;
  }

  switch(mode)
  {
  case gccull_back:
    glCullFace(GL_BACK);
    break;
  case gccull_front:
    glCullFace(GL_FRONT);
    break;
  }

  glEnable(GL_CULL_FACE);
}

//DEPTH
void kgmOGL::gcDepth(bool depth, bool mask, u32 mode)
{
  if(depth)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  if(mask)
    glDepthMask(GL_TRUE);
  else
    glDepthMask(GL_FALSE);

  glDepthFunc(gl_enum(mode));
}

//TEXTURE
void* kgmOGL::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
  GLu32  tex = 0, frame = 0;
  GLenum pic_fmt;
  GLu32  fmt_bt = 0;

#ifdef DEBUG

  kgm_log() << "gcGenTexture " << (s32)w << " " << (s32)h << " " << (s32)fmt << "\n";

#endif

#ifdef ANDROID

  (void)fmt_bt;

#endif

  switch(fmt)
  {
  case gctex_fmt8:
    pic_fmt = GL_RED;
    fmt_bt = 1;
    break;
  case gctex_fmt16:
    pic_fmt = GL_RGB;
    fmt_bt = 2;
    break;
  case gctex_fmt24:
    pic_fmt = GL_RGB;

#ifdef GL_COMPRESSED_RGB_ARB

    fmt_bt = GL_COMPRESSED_RGB_ARB;

#else

    fmt_bt = 3;

#endif

    break;
  case gctex_fmt32:
    pic_fmt = GL_RGBA;

#ifdef GL_COMPRESSED_RGBA_ARB

    fmt_bt = GL_COMPRESSED_RGBA_ARB;

#else

    fmt_bt = 4;

#endif

    break;

#ifdef GL_DEPTH_COMPONENT

  case gctex_fmtdepth:
    pic_fmt = GL_DEPTH_COMPONENT;
    fmt_bt = GL_DEPTH_COMPONENT;
    break;

#endif

  }

  switch(type)
  {
  case gctype_tex2d:
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    break;
#ifdef GL_TEXTURE_CUBE_MAP

  case gctype_texcube:
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
    break;

#endif

#ifdef GL_FRAMEBUFFER

  case gctype_textarget:
    glGenFramebuffers(1, &frame);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    break;

#endif

  }

  if(tex == 0)
  {
#ifdef DEBUG

    kgm_log() << "gl: no generated texture";

#endif

    return null;
  }

  const  GLubyte* p_str = null;
  GLenum err = 0;

#ifdef ANDROID

  (void)p_str;
  (void)err;

#endif

  switch(type)
  {
  case gctype_tex2d:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);

#ifdef GL_DEPTH_TEXTURE_MODE

    if(fmt == gctex_fmtdepth)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    }

#endif

#ifdef ANDROID

    glTexImage2D(GL_TEXTURE_2D, 0, pic_fmt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);

#else

    glTexImage2D(GL_TEXTURE_2D, 0, fmt_bt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);

#endif

#ifdef DEBUG
#ifdef GL_TEXTURE_COMPRESSED_ARB
    int compressed;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, &compressed);

    if (compressed == GL_TRUE)
    {
      kgm_log() << "OK \n";
    }
    else
    {
      kgm_log() << "NO \n";
    }

    err = glGetError();

    if(GL_NO_ERROR != err)
      kgm_log() << "gcGenTexture has error: " << (s32)err << "\n";
#endif
#endif
    break;
#ifdef GL_TEXTURE_CUBE_MAP
  case gctype_texcube:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,NULL);
    glDisable(GL_TEXTURE_CUBE_MAP);
    break;
#endif
  case gctype_textarget:
    if(fmt == gctex_fmtdepth)
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#ifndef ANDROID
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
#endif
#ifdef GL_DEPTH_ATTACHMENT
      glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
      //  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_INT, NULL);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);
#endif
    }
    else
    {
#ifdef GL_FRAMEBUFFER
      glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, NULL);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
#endif
    }
    break;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

#ifdef GL_FRAMEBUFFER
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

  Texture* t = new Texture;
  t->width = w;
  t->height = h;
  t->texture = tex;
  t->buffer = frame;
  t->format = fmt;
  t->type = type;

#ifdef DEBUG
  kgm_log() << "gcGenTexture generated tex: " << (s32)tex << "\n";
#endif

  return (void*)t;
}

void kgmOGL::gcFreeTexture(void *t)
{
  if(!t)
    return;

  if(((Texture*)t)->texture)
    glDeleteTextures(1,&((Texture*)t)->texture);

#ifdef GL_FRAMEBUFFER
  if(((Texture*)t)->buffer)
    glDeleteFramebuffers(1, &((Texture*)t)->buffer);
#endif

  delete (Texture*)t;
}

void kgmOGL::gcSetTexture(u32 stage, void* t)
{
#ifdef DEBUG
  GLenum err;
#endif

#ifdef ANDROID
  //glEnable(GL_TEXTURE_2D);
#endif

  if(!t)
  {
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(GL_TEXTURE_2D, 0);

#ifdef DEBUG
    err = glGetError();

    if(err != GL_NO_ERROR)
    {
      //kgm_log() << "gcSetTexture to 0 has error: " << (s32)err << "\n";
    }
#endif

    return;
  }

  glActiveTexture(GL_TEXTURE0 + stage);
  glBindTexture(GL_TEXTURE_2D, ((Texture*)t)->texture);

#ifdef DEBUG
  err = glGetError();

  if(err != GL_NO_ERROR)
  {
    //kgm_log() << "gcSetTexture has error: " << (s32)err << " tex: " << (s32)((Texture*)t)->texture << " stage: " << (s32)stage << "\n";
  }
#endif
}

//CLIP PLANE
void kgmOGL::gcClipPlane(bool en, u32 id, float* par)
{
  GLdouble c[4] = {par[0], par[1], par[2], par[3]};

#ifdef ANDROID
  (void)c;
#else
  glClipPlane(GL_CLIP_PLANE0 + id, c);
#endif
}

//STENCIL
void gcStencil(bool en, u32 func, u32 mask, u32 ref,
               u32 fail, u32 zfail, u32 zpass)
{

}

//Drawing
void kgmOGL::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
                    u32 i_size, u32 i_cnt, void *i_pnt)
{
  if(!v_pnt)
    return;

  unsigned char *pM = (unsigned char*)v_pnt;
  unsigned int  uv_size = sizeof(float) * 2;

  int ah = 0;

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  if(v_fmt & gcv_xyz)
  {
#ifdef GLES_1
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, v_size, pM);
#else
    ah = glGetAttribLocation(g_shader, "a_Vertex");
    glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
    glEnableVertexAttribArray(ah);
#endif

    pM += (sizeof(float) * 3);
  }

  if(v_fmt & gcv_nor)
  {
#ifdef GLES_1
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,v_size,pM);
#else
    ah = glGetAttribLocation(g_shader, "a_Normal");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 3, GL_FLOAT, GL_FALSE, v_size, pM);
#endif

    pM += (sizeof(float)*3);
  }

  if(v_fmt & gcv_col)
  {
#ifdef GLES_1
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4,GL_UNSIGNED_BYTE,v_size,pM);
#else
    ah = glGetAttribLocation(g_shader, "a_Color");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 4, GL_UNSIGNED_BYTE, GL_TRUE, v_size, pM);
#endif

    pM += sizeof(u32);
  }

  if(v_fmt & gcv_uv0)
  {
#ifdef GLES_1
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#else
    ah = glGetAttribLocation(g_shader, "a_UV");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#endif

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv1)
  {
#ifdef GLES_1
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#else
    ah = glGetAttribLocation(g_shader, "a_UV2");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#endif

    pM += (uv_size);
  }

  if(v_fmt & gcv_uv2)
  {
#ifdef GLES_1
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#else
    ah = glGetAttribLocation(g_shader, "a_UV3");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#endif

    pM += (uv_size);
  }

  if(v_fmt & gcv_bn0)
  {
#ifdef GLES_2
    ah = glGetAttribLocation(g_shader, "a_Weights");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
    ah = glGetAttribLocation(g_shader, "a_Indices");
    glEnableVertexAttribArray(ah);
    glVertexAttribPointer(ah, 4, GL_FLOAT, GL_FALSE, v_size, pM);
    pM += (4 * sizeof(float));
#else
    /*u32 k = ((u32)pM - (u32)v_pnt);
    float* f1 = (float*)pM;
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, v_size, pM);
    pM += (4 * sizeof(float));
    k = ((u32)pM - (u32)v_pnt);
    float* f2 = (float*)pM;
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(4, GL_FLOAT, v_size, pM);
    pM += (4 * sizeof(float));*/
#endif
  }

  if(i_pnt && i_cnt)
  {
    switch(i_size)
    {
    case 4:
#ifdef ANDROID
      glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
#else
      //glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
      glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_INT, i_pnt);
#endif
      break;
    default:
#ifdef ANDROID
      glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
#else
      //glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
      glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_SHORT, i_pnt);
#endif
    }
  }
  else
  {
    glDrawArrays(gl_enum(pmt), 0, v_cnt);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
  glDisableVertexAttribArray(5);
}

//VERTEX & INDEX BUFFER
void* kgmOGL::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize)
{
  VBO* vbo = new VBO;
  memset(vbo, 0, sizeof(VBO));

  if(vdata && vsize)
  {
    glGenBuffers(1, &vbo->vb);
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
    glBufferData(GL_ARRAY_BUFFER, vsize, vdata, GL_STATIC_DRAW);
  }

  if(idata && isize)
  {
    glGenBuffers(1, &vbo->ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STATIC_DRAW);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return (void*)vbo;
}

void  kgmOGL::gcFreeVertexBuffer(void* b)
{
  VBO* vbo = (VBO*)b;

  if(!vbo)
    return;

  if(vbo->vb)
    glDeleteBuffers(1, &vbo->vb);

  if(vbo->ib)
    glDeleteBuffers(1, &vbo->ib);

  delete vbo;
}

#define BUFFER_OFFSET(i) ((char*)NULL + (i))
void  kgmOGL::gcDrawVertexBuffer(void* b, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt,
                                 u32 isize, u32 icnt, u32 ioff)
{
  VBO* vbo = (VBO*)b;
  size_t offset = 0;
  u32 uvt = 2;

  if(!vbo)
    return;

#ifdef ANDROID
  (void)uvt;
  (void)offset;
#else
  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
#endif

  if(vbo->vb)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
  }

#ifdef GLES_2
#else
  if(vfmt & gcv_xyz)
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, vsize, (void*)offset);
    offset = offset + sizeof(float) * 3;
  }

  if(vfmt & gcv_nor)
  {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, vsize, (void*)offset);
    offset = offset + sizeof(float) * 3;
  }

  if(vfmt & gcv_col)
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4,GL_UNSIGNED_BYTE, vsize, (void*)offset);
    offset = offset + sizeof(u32);
  }
  // if(vfmt & gcv_spc){
  //  glEnableClientState(GL_COLOR_ARRAY);
  //   glColorPointer(4,GL_UNSIGNED_BYTE, vsize, (void*)offset);
  //    offset = offset + sizeof(u32);
  // }

  if(vfmt & gcv_uv0)
  {
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv1)
  {
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv2)
  {
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv3)
  {
    glClientActiveTexture(GL_TEXTURE3);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv4)
  {
    glClientActiveTexture(GL_TEXTURE4);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv5)
  {
    glClientActiveTexture(GL_TEXTURE5);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv6)
  {
    glClientActiveTexture(GL_TEXTURE6);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  if(vfmt & gcv_uv7)
  {
    glClientActiveTexture(GL_TEXTURE7);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  // glColor3f(1, 1, 1);
  if(vbo->ib && icnt)
  {
    //  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    switch(isize){
    case 4:
#ifdef ANDROID
      glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_INT, (void*)ioff);
#else
      glIndexPointer(GL_UNSIGNED_INT, 0, (void*) (long) ioff);
      glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, ((char*)0L + ioff));
#endif
      break;
    default:
      //   glIndexPointer(GL_UNSIGNED_SHORT, 0, 0);
      glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_SHORT, BUFFER_OFFSET(ioff));
      //   glDrawRangeElements (gl_enum(pmt), ioff, ioff + icnt, icnt/3, GL_UNSIGNED_SHORT, (void*)0);
      //   glDrawRangeElements (gl_enum(pmt), 0, vcnt - 1, icnt, GL_UNSIGNED_INT, (void*)ioff);//(ioff/1000));
    }
  }
  else
  {
    glDrawArrays(gl_enum(pmt), 0, vcnt);
  }

  glBindBuffer( GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

#ifndef ANDROID
  glPopClientAttrib();
#endif
}

// SHADERS
//GLint v_shad;
void* kgmOGL::gcGenShader(const char* vsrc, const char* fsrc)
{
  kgm_log() << "gcGenShader: Generating.\n";

  GLhandle prog = 0;
  GLhandle vshad = 0, fshad = 0;
  int stat[1] = {0};
  int size = 256;

#ifdef DEBUG
  char tbuf[256];
#endif

#ifdef GL_VERTEX_SHADER
  prog = glCreateProgramObject();

  if(GL_NO_ERROR != glGetError())
  {
#ifdef DEBUG
  kgm_log() << "gcGenShader error is " << (s32)glGetError() << "\n";
#endif

    return null;
  }

  //GL_VERTEX_SHADER
  if(vsrc)
  {
    size = strlen(vsrc);
    vshad = glCreateShaderObject(GL_VERTEX_SHADER);
    glShaderSource(vshad, 1, (const GLcharARB**)&vsrc, &size);
    glCompileShader(vshad);
    glGetObjectParameteriv(vshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
#ifdef DEBUG
#ifdef ANDROID
      glGetShaderInfoLog(vshad, 256, &size, tbuf);
#else
      glGetInfoLog(vshad, 256, &size, tbuf);
#endif
      kgm_log() << "VShader: " << (char*)tbuf << " " << (s32)strlen(tbuf) << "\n";
#endif

      glDeleteObject(vshad);
      fshad = 0;
    }
    else
    {
      glAttachObject(prog, vshad);
    }
  }

  if(fsrc)
  {
    size = strlen(fsrc);
    fshad = glCreateShaderObject(GL_FRAGMENT_SHADER);
    glShaderSource(fshad, 1, (const GLcharARB**)&fsrc, NULL);
    glCompileShader(fshad);
    glGetObjectParameteriv(fshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE)
    {
#ifdef DEBUG
#ifdef ANDROID
      glGetShaderInfoLog(fshad, 256, &size, tbuf);
#else
      glGetInfoLog(fshad, 256, &size, tbuf);
#endif
      kgm_log() << "FShader: " << (char*)tbuf << " " << (s32)strlen(tbuf) << "\n";
#endif
    }
    else
    {
      glAttachObject(prog, fshad);
    }
  }

  //glBindAttribLocation((GLhandle)prog, 0, "g_Vertex");
  //glBindAttribLocation((GLhandle)prog, 1, "g_Normal");
  //glBindAttribLocation((GLhandle)prog, 2, "g_Color");
  //glBindAttribLocation((GLhandle)prog, 3, "g_Texcoord");

  glLinkProgram(prog);

  glGetObjectParameteriv(prog, GL_OBJECT_LINK_STATUS, stat);

  if(stat[0] == GL_FALSE)
  {
#ifdef DEBUG
    glGetInfoLog(prog, 256, &size, tbuf);
    kgmLog::log(kgmString("LogARB: ") + kgmString(tbuf));
#endif

    glDeleteObject(vshad);
    glDeleteObject(fshad);
    glDeleteObject(prog);

    return null;
  }

#ifdef ANDROID

#define glDetachObject glDetachShader

#endif

  glDetachObject(prog, vshad);
  glDeleteObject(vshad);

  glDetachObject(prog, fshad);
  glDeleteObject(fshad);

#endif

  kgm_log() << "Generate shader " << (int) prog << ".\n";

  return (void*) (long) prog;
}

void kgmOGL::gcFreeShader(void* s)
{
#ifdef GL_VERTEX_SHADER
  size_t shader = (size_t)s;

  if(shader)
  {
    kgm_log() << "Delete shader " << (int) shader << ".\n";
    glDeleteObject((GLhandle)shader);
  }
#endif
}

void kgmOGL::gcSetShader(void* s)
{
#ifdef GL_VERTEX_SHADER
  if(s)
  {
    glUseProgramObject((GLhandle)(size_t)s);
    g_shader = (GLhandle)(size_t)s;
  }
  else
  {
    glUseProgramObject(0);

    g_shader = null;
  }
#endif
}

void  kgmOGL::gcBindAttribute(void* s, int i, const char* attr)
{
#ifdef GL_VERTEX_SHADER
  if(s)
  {
    glBindAttribLocation((GLhandle)(size_t)s, i, attr);
#ifdef DEBUG
#ifndef ANDROID
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
      //kgm_log() << "Error glBindAttribLocation: " << (s8*)gluErrorString(err) << " i " << (s32)i << "\n";
    }
#endif
#endif
  }
#endif
}

void kgmOGL::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{
#ifdef GL_VERTEX_SHADER
  GLint link = glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type){
  case gcunitype_float1:
    glUniform1fv(link, cnt, (float*)val);
    break;
  case gcunitype_float2:
    glUniform2fv(link, cnt, (float*)val);
    break;
  case gcunitype_float3:
    glUniform3fv(link, cnt, (float*)val);
    break;
  case gcunitype_float4:
    glUniform4fv(link, cnt, (float*)val);
    break;
  case gcunitype_int1:
    glUniform1iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int2:
    glUniform2iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int3:
    glUniform3iv(link, cnt, (const int*)val);
    break;
  case gcunitype_int4:
    glUniform4iv(link, cnt, (const int*)val);
    break;
  }
#endif
}

void kgmOGL::gcUniformMatrix(void* s, u32 type, u32 cnt, u32 trn, const char* par, void* val)
{
#ifdef GL_VERTEX_SHADER
  GLint link = glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  switch(type)
  {
  case gcunitype_mtx2:
    glUniformMatrix2fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx3:
    glUniformMatrix3fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  case gcunitype_mtx4:
    glUniformMatrix4fv(link, cnt, (GLboolean)trn, (float*)val);
    break;
  }
#endif
}

void kgmOGL::gcUniformSampler(void* s, const char* par, void* val)
{
#ifdef GL_VERTEX_SHADER
  GLint link = glGetUniformLocation((GLhandle)(size_t)s, par);

  if(link < 0)
    return;

  glUniform1i(link, (GLu32)(size_t)val);
#endif
}

#ifdef DEBUG
void kgmOGL::gcGetUniform(void* s, const char* par, void* val)
{
#ifdef GL_VERTEX_SHADER
  glGetUniformfv((GLhandle)(size_t)s, (GLint)(size_t)par, (float*)val);
#endif
}
#endif

