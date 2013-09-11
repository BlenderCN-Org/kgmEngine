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

GLu32          m_rendertarget = 0;

#ifdef ANDROID
EGLDisplay  m_display       = EGL_NO_DISPLAY;
EGLSurface  m_renderSurface = EGL_NO_SURFACE;
EGLContext  m_context       = EGL_NO_CONTEXT;
#endif

kgmOGL::kgmOGL(kgmOGLWindow *wnd){
  m_is_shader      = 0;
  m_is_framebuffer = 0;

  if(!wnd)
    return;
  
  this->m_wnd = wnd;

  GLubyte* oglVersion = (GLubyte*)glGetString(GL_VERSION);
  kgm_log() << "OpenGL Version: " << (char*)oglVersion << "\n";


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

#ifdef WIN32
  //if(wglSwapIntervalEXT) wglSwapIntervalEXT(0);
#endif

#ifdef LINUX
  // if(glxSwapIntervalSGI) glxSwapIntervalSGI(0);
#endif
  //init local values

  glEnable(GL_TEXTURE_2D);

  m_renderbuffer = 0;
  m_lights       = 0;
}

kgmOGL::~kgmOGL(){
  m_rendertarget = 0;
}

void kgmOGL::gcSetParameter(u32 param, void* value){
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
  }
}
void kgmOGL::gcGetParameter(u32 param, void* value){
  switch(param)
  {
  case gcsup_shaders:
    *((u32*)value) = m_is_shader;
    break;
  case gcsup_fbuffers:
    *((u32*)value) = m_is_framebuffer;
    break;
  }
}
void kgmOGL::gcClear(u32 flag, u32 col, float depth, u32 sten){
  GLu32 cl = 0;

  if(flag & gcflag_color){
    cl |= GL_COLOR_BUFFER_BIT;
    float inv = 1.0f / 255.0f;
    float a = inv * ((col & 0xff000000) >> 24),
        r = inv * ((col & 0x00ff0000) >> 16),
        g = inv * ((col & 0x0000ff00) >> 8),
        b = inv *  (col & 0x000000ff);
    glClearColor(r, g, b, a);
  }

  if(flag & gcflag_depth){
    cl |= GL_DEPTH_BUFFER_BIT;
    glClearDepth(depth);
  }

  if(flag & gcflag_stencil){
    cl |= GL_STENCIL_BUFFER_BIT;
    glClearStencil(sten);
  }
  glClear(cl);
}

void kgmOGL::gcBegin(){
}

void kgmOGL::gcEnd(){
}

void kgmOGL::gcRender(){
#ifdef WIN32
  SwapBuffers(m_wnd->m_hdc);
#endif

#ifdef LINUX
  glXSwapBuffers(m_wnd->m_dpy, m_wnd->m_wnd);
#endif

#ifdef ANDROIDXXX
  eglSwapBuffers(m_wnd->display, m_wnd->surface);
#endif
}

void kgmOGL::gcSetTarget(void* t){
#ifdef GL_FRAMEBUFFER
  if(!t || !((Texture*)t)->buffer){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, ((Texture*)t)->buffer);
  GLenum err = glGetError();
  if(err != GL_NO_ERROR){
    int k = 0;
  }
  GLint ipar = 0;
  glEnable(GL_DEPTH_TEST);
  glDepthMask(true);
  glDepthFunc(GL_LEQUAL);
  glGetIntegerv(GL_DEPTH_BITS, &ipar);
  if(ipar == 0){
    int k = 0;
  }
#endif
  //  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  //  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
  //  glDrawBuffer(GL_BACK);
  //  glReadBuffer(GL_BACK);
}

void kgmOGL::gcSetMatrix(u32 mode, float* mtx){
#ifdef GL_PROJECTION
  switch(mode){
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

void kgmOGL::gcGetMatrix(u32 mode, float* mtx){
#ifdef GL_PROJECTION
  switch(mode){
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

void kgmOGL::gcSetViewport(int x, int y, int w, int h, float n, float f){
  glViewport(x, y, w, h);
}

//Light
void kgmOGL::gcSetLight(int i, float* pos, float range){
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

 float col[] = {1, 1, 1, 1};
 glLightfv(GL_LIGHT0 + i, GL_POSITION, (float*)pos);
 glLightfv(GL_LIGHT0 + i, GL_AMBIENT,  (float*)col);
 glLightfv(GL_LIGHT0 + i, GL_DIFFUSE,  (float*)col);
 glLightf(GL_LIGHT0  + i, GL_LINEAR_ATTENUATION, 1.0f - range);
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
void  kgmOGL::gcBlend(bool en, u32 fnsrc, u32 fndst){
  if(!en){
    glDisable(GL_BLEND);
    return;
  }
  glBlendFunc(gl_enum(fnsrc), gl_enum(fndst));
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
}

//ALPHA
void  kgmOGL::gcAlpha(bool en, u32 fn, float ref){
#ifdef GL_ALPHA_TEST
    if(!en){
    glDisable(GL_ALPHA_TEST);
    return;
  }
  glAlphaFunc(gl_enum(fn), ref);
  glEnable(GL_ALPHA_TEST);
#endif
}

//CULL
void kgmOGL::gcCull(u32 mode){
  if(!mode){
    glDisable(GL_CULL_FACE);
    return;
  }
  switch(mode){
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
void kgmOGL::gcDepth(bool depth, bool mask, u32 mode){
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


//Dimension MODE
void kgmOGL::gc2DMode(){
  int  x, y, w, h;

#ifdef GL_PROJECTION
  m_wnd->getRect(x, y, w, h);
  glMatrixMode(GL_PROJECTION);
  //glGetFloatv(GL_PROJECTION_MATRIX, mtx_mode_proj);
  glLoadIdentity();
  glOrtho(0, w, h, 0, 1, -1);
  glMatrixMode(GL_MODELVIEW);
  //glGetFloatv(GL_MODELVIEW_MATRIX, mtx_mode_view);
  glLoadIdentity();
#endif
}
void kgmOGL::gc3DMode()
{
#ifdef GL_PROJECTION
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(mtx_mode_proj);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(mtx_mode_view);
#endif
}

//TEXTURE
void* kgmOGL::gcGenTexture(void *pd, u32 w, u32 h, u32 fmt, u32 type)
{
  GLu32 tex = 0, frame = 0;
  GLenum pic_fmt;
  GLu32 fmt_bt = 0;

  switch(fmt){
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
    fmt_bt = 3;
    break;
  case gctex_fmt32:
    pic_fmt = GL_RGBA;
    fmt_bt = 4;
    break;
#ifdef GL_DEPTH_COMPONENT
  case gctex_fmtdepth:
    pic_fmt = GL_DEPTH_COMPONENT;
    fmt_bt = GL_DEPTH_COMPONENT;
    break;
#endif
  default:
    pic_fmt = GL_RGB;
    fmt_bt = 3;
  }

#ifdef ANDROID
   glActiveTexture(GL_TEXTURE0);
#endif

  switch(type){
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
  default:
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
  }

  if(tex == 0)
  {
#ifdef TEST
    kgm_log() << "gl: no generated texture";
#endif

    return null;
  }


  const  GLubyte* p_str;
  GLenum err = 0;

  switch(type)
  {
  case gctype_tex2d:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

#ifdef TEST
    err = glGetError();

    if(GL_NO_ERROR != err)
    {
      u32 k = 0;

      switch(err)
      {
      case GL_INVALID_ENUM:
        kgm_log() << "gcGenTexture has error: GL_INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        kgm_log() << "gcGenTexture has error: GL_INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        kgm_log() << "gcGenTexture has error: GL_INVALID_OPERATION";
        break;
      default:
        kgm_log() << "gcGenTexture has error: Unknown";
      }
    }
#endif
    break;
    // case 1:
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_SGIX, GL_TRUE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_OPERATOR_SGIX, GL_TEXTURE_LEQUAL_R_SGIX);

    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE, 0.5);
    // glTexGeni(GL_TEXTURE_2D, GL_S, GL_EYE_LINEAR);
    // glTexGeni(GL_TEXTURE_2D, GL_T, GL_EYE_LINEAR);
    // glTexGeni(GL_TEXTURE_2D, GL_R, GL_EYE_LINEAR);
    // glTexGeni(GL_TEXTURE_2D, GL_Q, GL_EYE_LINEAR);
    //   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0,
    //              GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    //  break;
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
    if(fmt == gctex_fmtdepth){
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
  default:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt_bt, w, h, 0, pic_fmt, GL_UNSIGNED_BYTE, pd);
  }

#ifdef TEST
    err = glGetError();

    if(GL_NO_ERROR != err)
    {
      u32 k = 0;

      switch(err)
      {
      case GL_INVALID_ENUM:
        kgm_log() << "gcGenTexture has error: GL_INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        kgm_log() << "gcGenTexture has error: GL_INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        kgm_log() << "gcGenTexture has error: GL_INVALID_OPERATION";
        break;
      default:
        kgm_log() << "gcGenTexture has error: Unknown";
      }
    }
#endif

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

  return (void*)t;
}

void kgmOGL::gcFreeTexture(void *t){
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
  GLenum err;

  if(!t)
  {
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(GL_TEXTURE_2D, 0);

    return;
  }

  glActiveTexture(GL_TEXTURE0 + stage);
  glBindTexture(GL_TEXTURE_2D, ((Texture*)t)->texture);

#ifdef TEST
  err = glGetError();

  if(err != GL_NO_ERROR)
  {
    kgm_log() << "gcSetTexture has error: " << (s32)err << "\n";
  }
#endif
}

/*
//RENDER BUFFER 
void* kgmOGL::gcGenRenderTarget(u32 w, u32 h, u32 flags){
 GLenum stat = 0;
 RenderBuffer* rb = new RenderBuffer;
 memset(rb, 0, sizeof(RenderBuffer));


 glGenFramebuffersEXT(1, &rb->frame);
 glBindFramebufferEXT(GL_FRAMEBUFFER, rb->frame);

 if(flags & 0x01){
  glGenTextures(1, &rb->color);
  glBindTexture(GL_TEXTURE_2D, rb->color);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rb->color, 0);
 }
 if(flags & 0x02){
  glGenTextures(1, &rb->depth);
  glBindTexture(GL_TEXTURE_2D, rb->depth);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_INT, NULL);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

//  glDrawBuffer(GL_NONE);
//  glReadBuffer(GL_NONE);
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, rb->depth, 0);
 }

 stat = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
 if(stat != GL_FRAMEBUFFER_COMPLETE_EXT){
  framebuffer_status(stat);
 }
 glBindTexture(GL_TEXTURE_2D, 0);
 glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

 return rb;
}

void kgmOGL::gcFreeRenderTarget(void* rb){
 if(!rb)
   return;
 if( ((RenderBuffer*)rb)->color )
  glDeleteTextures(1, &((RenderBuffer*)rb)->color);
 if( ((RenderBuffer*)rb)->depth )
  glDeleteTextures(1, &((RenderBuffer*)rb)->depth);
 if( ((RenderBuffer*)rb)->frame )
  glDeleteFramebuffersEXT(1, &((RenderBuffer*)rb)->frame);
 delete (RenderBuffer*)rb;
}

void kgmOGL::gcSetRenderTarget(void* rb){
 if(!rb || !((RenderBuffer*)rb)->frame){
  glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
  return;
 }

 glBindFramebufferEXT(GL_FRAMEBUFFER, ((RenderBuffer*)rb)->frame);
 GLenum err = glGetError();
 if(err != GL_NO_ERROR){
  int k = 0;
 }
 GLint ipar = 0;
 glEnable(GL_DEPTH_TEST);
 glDepthMask(true);
 glDepthFunc(GL_LEQUAL);
 glGetIntegerv(GL_DEPTH_BITS, &ipar);
 if(ipar == 0){
  int k = 0;
 }
//  glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
//  glDrawBuffer(GL_BACK);
//  glReadBuffer(GL_BACK);
}

void kgmOGL::gcSetRenderTexture(void* rb, u32 stage, u32 flags){
 if(!rb || !((RenderBuffer*)rb)->frame){
  gcSetTexture(stage, 0);
  return;
 }

 if(flags & 0x01){
  glActiveTexture(GL_TEXTURE0 + stage);
  glBindTexture(GL_TEXTURE_2D, ((RenderBuffer*)rb)->color);
 }
 if(flags & 0x02){
  glActiveTexture(GL_TEXTURE0 + stage);
  glBindTexture(GL_TEXTURE_2D, ((RenderBuffer*)rb)->depth);
 }
}
*/
/*
void kgmOGL::gcCoordTexture(u32 coord, u32 mode, void* par){
 static GLenum   gl_coord = 0;
 static GLenum   gl_mode = 0;
 static GLenum	 gl_gen = 0;
 GLint    gl_ipar;
 GLfloat* gl_fpar;
 switch(coord){
 case gctex_s:
   gl_coord = GL_S;
   gl_gen = GL_TEXTURE_GEN_S;
   break;
 case gctex_t:
   gl_coord = GL_T;
   gl_gen = GL_TEXTURE_GEN_T;
   break;
 case gctex_r:
   gl_coord = GL_R;
   gl_gen = GL_TEXTURE_GEN_R;
   break;
 case gctex_q:
   gl_coord = GL_Q;
   gl_gen = GL_TEXTURE_GEN_Q;
   break;
 default:
   gl_coord = GL_S;
   gl_gen = GL_TEXTURE_GEN_S;
 }
 switch(mode){
 case gctex_gen:
   gl_mode = GL_TEXTURE_GEN_MODE;
   break;
 case gctex_obj:
   gl_mode = GL_OBJECT_PLANE;
   break;
 case gctex_eye:
   gl_mode = GL_EYE_PLANE;
   break;
 }

 if(mode == 0){
  glDisable(gl_gen);
  return;
 }else{
  glEnable(gl_gen);
 }

 if(gl_mode == GL_TEXTURE_GEN_MODE){
  switch((u32)par){
  case gctex_sphere:
    gl_ipar = GL_SPHERE_MAP;
    break;
  case gctex_obj:
    gl_ipar = GL_OBJECT_LINEAR;
    break;
  case gctex_eye:
    gl_ipar = GL_EYE_LINEAR;
    break;
  default:
    gl_ipar = GL_SPHERE_MAP;
  }
  glTexGeni(gl_coord, gl_mode, gl_ipar);
 }else{
  gl_fpar = (GLfloat*)par;
  glTexGenfv(gl_coord, gl_mode, gl_fpar);
 }
}
*/
//CLIP PLANE
void kgmOGL::gcClipPlane(bool en, u32 id, float* par){
  GLdouble c[4] = {par[0], par[1], par[2], par[3]};

#ifndef ANDROID
  glClipPlane(GL_CLIP_PLANE0 + id, c);
#endif
}
//STENCIL
void gcStencil(bool en, u32 func, u32 mask, u32 ref, 
               u32 fail, u32 zfail, u32 zpass){

}
//Drawing
void kgmOGL::gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt,
                    u32 i_size, u32 i_cnt, void *i_pnt){
  if(!v_pnt)
      return;

  unsigned char *pM = (unsigned char*)v_pnt;
  unsigned int  p_size  = sizeof(float) * 3;
  unsigned int  n_size  = sizeof(float) * 3;
  unsigned int  c_size  = sizeof(unsigned );
  unsigned int  uv_size = sizeof(float) * 2;

  if(v_fmt & gcv_xyz)
  {
#ifdef GLES_2
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, v_size, pM);
#endif
    vec3 pos = *((vec3*)pM);
    pM += (sizeof(float) * 3);
  }
  if(v_fmt & gcv_nor)
  {
#ifdef GLES_2
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,v_size,pM);
#endif
    pM += (sizeof(float)*3);
  }

  if(v_fmt & gcv_col)
  {
#ifdef GLES_2
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, v_size, pM);
#else
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4,GL_UNSIGNED_BYTE,v_size,pM);
#endif
    pM += sizeof(u32);
  }
  // if(v_fmt & gcv_spc){
  //  glEnableClientState(GL_COLOR_ARRAY);
  //   glColorPointer(4,GL_UNSIGNED_BYTE,v_size,pM);
  //  pM += sizeof(u32);
  // }
  if(v_fmt & gcv_uv0)
  {
#ifdef GLES_2
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#endif
    pM += (uv_size);
  }

  if(v_fmt & gcv_uv1){
#ifdef GLES_2
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#endif
    pM += (uv_size);
  }

  if(v_fmt & gcv_uv2){
#ifdef GLES_2
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#endif
    pM += (uv_size);
  }

  if(v_fmt & gcv_uv3){
#ifdef GLES_2
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glClientActiveTexture(GL_TEXTURE3);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#endif
    pM += (uv_size);
  }

  if(v_fmt & gcv_uv4){
#ifdef GLES_2
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glClientActiveTexture(GL_TEXTURE4);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#endif
    pM += (uv_size);
  }

  if(v_fmt & gcv_uv5){
#ifdef GLES_2
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    glClientActiveTexture(GL_TEXTURE5);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,v_size,pM);
#endif
    pM += (uv_size);
  }

  if(v_fmt & gcv_bn0){
#ifdef GLES_2
//    glEnableVertexAttribArray(8);
//    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, v_size, pM);
#else
    u32 k = ((u32)pM - (u32)v_pnt);
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
    pM += (4 * sizeof(float));
#endif
  }

//  glColor3f(1, 1, 1);
  if(i_pnt && i_cnt){
    switch(i_size){
    case 4:
#ifdef ANDROID
      glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_INT, i_pnt);
#else
      glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_INT, i_pnt);
#endif
      break;
    default:
#ifdef ANDROID
      glDrawElements(gl_enum(pmt),i_cnt,GL_UNSIGNED_SHORT,i_pnt);
#else
      glDrawRangeElements(gl_enum(pmt),0, v_cnt - 1, i_cnt,GL_UNSIGNED_SHORT, i_pnt);
#endif
    }
  }else{
    glDrawArrays(gl_enum(pmt), 0, v_cnt);
  }

#ifdef GLES_2
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
  glDisableVertexAttribArray(5);
#else
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
}


void kgmOGL::gcDrawRect(int x, int y, int w, int h, u32 col){
  typedef struct{  vec3 pos;  u32 col;  vec2 uv; } V;
  V v[4];

  gc2DMode();
  /*
  v[0].pos = vec3(rect.min.x, rect.min.y, 0); v[0].col = col; v[0].uv = vec2(0, 1);
  v[1].pos = vec3(rect.min.x, rect.max.y, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(rect.max.x, rect.max.y, 0); v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(rect.max.x, rect.min.y, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  gcDraw(gcpmt_trianglefan, gcv_xyz | gcv_dif | gcv_tex, sizeof(V), 4, v, 0, 0);
*/
  v[0].pos = vec3(x,     y, 0);     v[0].col = col; v[0].uv = vec2(0, 0);
  v[1].pos = vec3(x,     y + h, 0); v[1].col = col; v[1].uv = vec2(0, 1);
  v[2].pos = vec3(x + w, y, 0);     v[2].col = col; v[2].uv = vec2(1, 0);
  v[3].pos = vec3(x + w, y + h, 0); v[3].col = col; v[3].uv = vec2(1, 1);

  gcDraw(gcpmt_trianglestrip, gcv_xyz | gcv_col | gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
  gc3DMode();
}

void  kgmOGL::gcDrawBillboard(vec3 pos, float w, float h, u32 col){
  mtx4 mv, mp, m;
  vec3 rv, uv;
  typedef struct{ vec3 pos; u32 dif; vec2 uv; } V;
  //  typedef struct{ vec3 pos; vec2 uv; } V;

  V v[4];

  gcGetMatrix(gcmtx_view, mv.m);
  gcGetMatrix(gcmtx_proj, mp.m);
  m = mv * mp;

  rv = vec3(m.m[0], m.m[4], m.m[8]); rv.normalize();
  rv.x *= w * 0.5f;
  rv.y *= w * 0.5f;
  rv.z *= w * 0.5f;
  uv = vec3(m.m[1], m.m[5], m.m[9]); uv.normalize();
  uv.x *= h * 0.5f;
  uv.y *= h * 0.5f;
  uv.z *= h * 0.5f;

  v[0].pos = pos - rv + uv;
  v[0].dif = col;
  v[0].uv.x = 0.0f, v[0].uv.y = 0.0f;
  v[1].pos = pos - rv - uv;
  v[1].dif = col;
  v[1].uv.x = 0.0f, v[1].uv.y = 1.0f;
  v[2].pos = pos + rv + uv;
  v[2].dif = col;
  v[2].uv.x = 1.0f, v[2].uv.y = 0.0f;
  v[3].pos = pos + rv - uv;
  v[3].dif = col;
  v[3].uv.x = 1.0f, v[3].uv.y = 1.0f;

  m.identity();
  gcSetMatrix(gcmtx_view, m.m);
  gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(V), 4, v, 0, 0, 0);
  //  glColor4f(1, 1, 1, 1);
}

//VERTEX & INDEX BUFFER 
void* kgmOGL::gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize){
  VBO* vbo = new VBO;
  memset(vbo, 0, sizeof(VBO));

  if(vdata && vsize){
    glGenBuffers(1, &vbo->vb);
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
    glBufferData(GL_ARRAY_BUFFER, vsize, vdata, GL_STATIC_DRAW);
  }
  if(idata && isize){
    glGenBuffers(1, &vbo->ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STATIC_DRAW);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return (void*)vbo;
}
void  kgmOGL::gcFreeVertexBuffer(void* b){
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
                                 u32 isize, u32 icnt, u32 ioff){
  VBO* vbo = (VBO*)b;
  u32 offset = 0;
  u32 uvt = 2;

  if(!vbo)
    return;

#ifndef ANDROID
  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
#endif

  if(vbo->vb){
    glBindBuffer(GL_ARRAY_BUFFER, vbo->vb);
  }

#ifdef GLES_2
#else
  if(vfmt & gcv_xyz){
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, vsize, (void*)offset);
    offset = offset + sizeof(float) * 3;
  }
  if(vfmt & gcv_nor){
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, vsize, (void*)offset);
    offset = offset + sizeof(float) * 3;
  }
  if(vfmt & gcv_col){
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4,GL_UNSIGNED_BYTE, vsize, (void*)offset);
    offset = offset + sizeof(u32);
  }
  // if(vfmt & gcv_spc){
  //  glEnableClientState(GL_COLOR_ARRAY);
  //   glColorPointer(4,GL_UNSIGNED_BYTE, vsize, (void*)offset);
  //    offset = offset + sizeof(u32);
  // }
  if(vfmt & gcv_uv0){
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv1){
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv2){
    glClientActiveTexture(GL_TEXTURE2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv3){
    glClientActiveTexture(GL_TEXTURE3);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv4){
    glClientActiveTexture(GL_TEXTURE4);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv5){
    glClientActiveTexture(GL_TEXTURE5);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv6){
    glClientActiveTexture(GL_TEXTURE6);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }
  if(vfmt & gcv_uv7){
    glClientActiveTexture(GL_TEXTURE7);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2,GL_FLOAT,vsize, (void*)offset);
    offset = offset + sizeof(float) * uvt;
  }

  // glColor3f(1, 1, 1);
  if(vbo->ib && icnt){
    //  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->ib);
    switch(isize){
    case 4:
#ifdef ANDROID
      glDrawElements(gl_enum(pmt), icnt, GL_UNSIGNED_INT, (void*)ioff);
#else
      glIndexPointer(GL_UNSIGNED_INT, 0, (void*)ioff);
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
  else{
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
GLint v_shad;
void* kgmOGL::gcGenShader(const char* vsrc, const char* fsrc){
  GLhandle prog = 0;
  GLhandle vshad = 0, fshad = 0;
  int stat[1] = {0};
  int size = 256;
  char tbuf[256];

#ifdef GL_VERTEX_SHADER
  prog = glCreateProgramObject();

  //GL_VERTEX_SHADER
  if(vsrc){
    size = strlen(vsrc);
    v_shad = vshad = glCreateShaderObject(GL_VERTEX_SHADER);
    glShaderSource(vshad, 1, (const GLcharARB**)&vsrc, &size);
    glCompileShader(vshad);
    glGetObjectParameteriv(vshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE){
      glGetInfoLog(vshad, 256, &size, tbuf);
#ifdef WIN32
      MessageBox(0, tbuf, "VShader", 0);
#elif defined LINUX
      kgmLog::log(kgmString("VShader: %s") + kgmString(tbuf));
#endif
    }
    glAttachObject(prog, vshad);
    //  glDeleteObject(vshad);
  }

  if(fsrc){
    size = strlen(fsrc);
    fshad = glCreateShaderObject(GL_FRAGMENT_SHADER);
    glShaderSource(fshad, 1, (const GLcharARB**)&fsrc, NULL);
    glCompileShader(fshad);
    glAttachObject(prog, fshad);
    glGetObjectParameteriv(fshad, GL_OBJECT_COMPILE_STATUS, stat);

    if(stat[0] == GL_FALSE){
      glGetInfoLog(fshad, 256, &size, tbuf);
#ifdef WIN32
      MessageBox(0, tbuf, "FShader", 0);
#elif defined LINUX
      kgmLog::log(kgmString("FShader: ") + kgmString(tbuf));
#endif
    }
    glAttachObject(prog, fshad);
    //  glDeleteObject(fshad);
  }

  glLinkProgram(prog);
  glGetObjectParameteriv(prog, GL_OBJECT_LINK_STATUS, stat);
  if(stat[0] == GL_FALSE){
    glGetInfoLog(prog, 256, &size, tbuf);
    kgmLog::log(kgmString("LogARB: ") + kgmString(tbuf));
  }
  // glUseProgram();
#endif

  return (void*)prog;
}
void kgmOGL::gcFreeShader(void* s){
#ifdef GL_VERTEX_SHADER
  // glDetachObject(((ShadeStruct*)shad), ((ShadeStruct*)shad));
  if(s){
    glDeleteObject((GLhandle)s);
  }
#endif
}
void kgmOGL::gcSetShader(void* s){
#ifdef GL_VERTEX_SHADER
  if(s){
    glUseProgramObject((GLhandle)s);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
#ifndef ANDROID
      const GLubyte* ers = gluErrorString(err);
      kgmLog::log(kgmString("some error fixed ") + kgmString((const char*)ers));
#endif
    }
  }
  else{
    glUseProgramObject(0);
  }
#endif
}

void  kgmOGL::gcBindAttribute(void* s, int i, const char* attr)
{
#ifdef GL_VERTEX_SHADER
  if(s)
  {
    glBindAttribLocation((GLhandle)s, i, attr);
    GLenum err = glGetError();

    if(err != GL_NO_ERROR)
      kgm_log() << "Error glBindAttribLocation: " << (s32)err << "\n";
  }
#endif
}

void kgmOGL::gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val)
{ 
#ifdef GL_VERTEX_SHADER
  GLint link = glGetUniformLocation((GLhandle)s, par);
  if(link < 0)
    return;

  if(type == 0){
    switch(cnt){
    case 1:
      glUniform1fv(link, 1, (float*)val);
      break;
    case 2:
      glUniform2fv(link, 1, (float*)val);
      break;
    case 3:
      glUniform3fv(link, 1, (float*)val);
      break;
    case 4:
      glUniform4fv(link, 1, (float*)val);
      break;
    }
  }else if(type == 1){
    switch(cnt){
    case 1:
      glUniform1iv(link, 1, (const int*)val);
      break;
    case 2:
      glUniform2iv(link, 1, (const int*)val);
      break;
    case 3:
      glUniform3iv(link, 1, (const int*)val);
      break;
    case 4:
      glUniform4iv(link, 1, (const int*)val);
      break;
    }
  }
#endif
}

void kgmOGL::gcUniformMatrix(void* s, u32 cnt, u32 trn, const char* par, void* val)
{ 
#ifdef GL_VERTEX_SHADER
  GLint link = glGetUniformLocation((GLhandle)s, par);
  if(link < 0)
    return;
  switch(cnt){
  case 2:
    glUniformMatrix2fv(link, 1, (GLboolean)trn, (float*)val);
    break;
  case 3:
    glUniformMatrix3fv(link, 1, (GLboolean)trn, (float*)val);
    break;
  case 4:
    glUniformMatrix4fv(link, 1, (GLboolean)trn, (float*)val);
    break;
  }
#endif
}

void kgmOGL::gcUniformSampler(void* s, const char* par, void* val){
#ifdef GL_VERTEX_SHADER
  GLint link = glGetUniformLocation((GLhandle)s, par);

  if(link < 0)
    return;
  glUniform1i(link, (GLu32)val);
#endif
}

