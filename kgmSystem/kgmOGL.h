// kgmOGLDevice.h: interface for the kgmOGLDevice class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../kgmBase/kgmIGraphics.h"
#include "kgmWindow.h"

#ifdef WIN32
 #include <windows.h>
#elif defined LINUX
#endif

#ifdef ANDROID
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

//#include "inc/glext.h"


class kgmOGLWindow;

//////// OPENGL GRAPHYCAL CONTEXT
class kgmOGL: public kgmIGraphics{
public:
//VERTEX & INDEX BUFFER 
struct VBO{
 GLu32 vb, ib;
 u32   vsize, isize;
};
//FRAME BUFFER 
struct RenderBuffer{
  GLu32 frame, color, depth, stencil;
  u16 width, height;
};

struct Texture{
 u32   type;
 GLu32 buffer, texture;
 u16 width,  height, format;
};
private:
 kgmOGLWindow* m_wnd;
 
//Current Render Buffer
 RenderBuffer* m_renderbuffer;
// Mode Matrices
 float mtx_mode_view[16], mtx_mode_proj[16];
public:
 kgmOGL(kgmOGLWindow* wnd);
 virtual ~kgmOGL();


//RENDER
 void  gcSetParameter(u32 param, void* value);
 void  gcGetParameter(u32 param, void* value);
 void  gcClear(u32 flag, u32 col, f32 depth, u32 sten);
 void  gcBegin();
 void  gcEnd();
 void  gcRender();
 void  gcSetTarget(void* t);
//MODE
 void  gc2DMode();
 void  gc3DMode();
//DRAW
 void  gcDraw(u32 pmt, u32 v_fmt, u32 v_size, u32 v_cnt, void *v_pnt, u32 i_size, u32 i_cnt, void *i_pnt);
 void  gcDrawRect(int x, int y, int w, int h, u32 col);
 //void  gcDrawText(lRect clip, float w, float h, u32 col, char *text, u32 tlen);
 void  gcDrawBillboard(vec3 v, float w, float h, u32 col);
//TEXTURE
 void* gcGenTexture(void *mf, u32 w, u32 h, u32 bpp, u32 type);
 void  gcFreeTexture(void* tex);
 void  gcSetTexture(u32 stage, void* tex);
//MATRIX
 void  gcSetMatrix(u32 mm, float* mtx);
 void  gcGetMatrix(u32 mm, float* mtx);
//VIEWPORT
 void  gcSetViewport(int x, int y, int w, int h, float n, float f);
 //BLEND
 void  gcBlend(bool enabla, u32 fnsrc, u32 fndst);
 //ALPHA
 void  gcAlpha(bool en, u32 fn, float ref);
 //CULL
 void  gcCull(u32 mode);
 //DEPTH
 void  gcDepth(bool en, bool mask, u32 mode);
 //CLIP PLANE
 void  gcClipPlane(bool, u32 id, float*);
 //STENCIL
 void  gcStencil(bool en, u32 func, u32 mask, u32 ref, 
		 u32 fail, u32 zfail, u32 zpass); 
 //VERTEX & INDEX BUFFER 
 void* gcGenVertexBuffer(void* vdata, u32 vsize, void* idata, u32 isize);
 void  gcFreeVertexBuffer(void*);
 void  gcDrawVertexBuffer(void* buf, u32 pmt, u32 vfmt, u32 vsize, u32 vcnt, u32 isize, u32 icnt, u32 ioff);
 // SHADER
 void* gcGenShader(const char*, const char*);
 void  gcFreeShader(void* s);
 void  gcSetShader(void* s);
 void  gcUniform(void* s, u32 type, u32 cnt, const char* par, void* val);
 void  gcUniformMatrix(void* s, u32, u32, const char*, void*);
 void  gcUniformSampler(void* s, const char*, void*);

 //////////////////////////// inlines
 GLenum gl_enum(u32 e){
  switch(e){
  case gccmp_always:    return GL_ALWAYS;
  case gccmp_never:	return GL_NEVER;
  case gccmp_equal:	return GL_EQUAL;
  case gccmp_nequal:    return GL_NOTEQUAL;
  case gccmp_less:      return GL_LESS;
  case gccmp_lequal:    return GL_LEQUAL;
  case gccmp_great:	return GL_GREATER;
  case gccmp_gequal:    return GL_GEQUAL;

  case gcblend_zero:		return GL_ZERO;
  case gcblend_one:		return GL_ONE;
  case gcblend_dstcol:		return GL_DST_COLOR;
  case gcblend_srccol:		return GL_SRC_COLOR;
  case gcblend_dsticol:		return GL_ONE_MINUS_DST_COLOR;
  case gcblend_srcicol:		return GL_ONE_MINUS_SRC_COLOR;
  case gcblend_dstalpha:	return GL_DST_ALPHA;
  case gcblend_srcalpha:	return GL_SRC_ALPHA;
  case gcblend_dstialpha:	return GL_ONE_MINUS_DST_ALPHA;
  case gcblend_srcialpha:	return GL_ONE_MINUS_SRC_ALPHA;

  case gcpmt_points:		 return  GL_POINTS;
  case gcpmt_lines:			 return  GL_LINES;
  case gcpmt_triangles:		 return  GL_TRIANGLES;
  case gcpmt_linestrip:      return  GL_LINE_STRIP;
  case gcpmt_trianglestrip:  return  GL_TRIANGLE_STRIP;
  case gcpmt_trianglefan:    return  GL_TRIANGLE_FAN;
  }
  return GL_ZERO;
 }
 GLenum gl_blend(gc_enum e){
  switch(e){
  case gcblend_zero:		return GL_ZERO;
  case gcblend_one:		return GL_ONE;
  case gcblend_dstcol:		return GL_DST_COLOR;
  case gcblend_srccol:		return GL_SRC_COLOR;
  case gcblend_dsticol:		return GL_ONE_MINUS_DST_COLOR;
  case gcblend_srcicol:		return GL_ONE_MINUS_SRC_COLOR;
  case gcblend_dstalpha:	return GL_DST_ALPHA;
  case gcblend_srcalpha:	return GL_SRC_ALPHA;
  case gcblend_dstialpha:	return GL_ONE_MINUS_DST_ALPHA;
  case gcblend_srcialpha:	return GL_ONE_MINUS_SRC_ALPHA;
  }
  return GL_ZERO;
 }
 GLenum gl_primitive(gc_enum e){
  switch(e){
  case gcpmt_points:         return  GL_POINTS;
  case gcpmt_lines:          return  GL_LINES;
  case gcpmt_triangles:      return  GL_TRIANGLES;
  case gcpmt_linestrip:      return  GL_LINE_STRIP;
  case gcpmt_trianglestrip:  return  GL_TRIANGLE_STRIP;
  case gcpmt_trianglefan:    return  GL_TRIANGLE_FAN;
  }
  return GL_ZERO;
 }
};

