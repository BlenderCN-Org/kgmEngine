#include "kgmOGLExt.h"
#include "../kgmBase/kgmLog.h"

#ifndef ANDROID
//DRAW ELEMENTS
PFNGLDRAWRANGEELEMENTSPROC		glDrawRangeElements = 0;
//TEXTURE EXTENTIONS
PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB = 0;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = 0;

//VERTEX BUFFER EXTENTIONS
PFNGLGENBUFFERSARBPROC			glGenBuffersARB = 0;
PFNGLDELETEBUFFERSARBPROC		glDeleteBuffersARB = 0;
PFNGLBINDBUFFERARBPROC			glBindBufferARB = 0;
PFNGLBUFFERDATAARBPROC			glBufferDataARB = 0;
PFNGLBUFFERSUBDATAARBPROC		glBufferSubDataARB = 0;
PFNGLGETBUFFERSUBDATAARBPROC	glGetBufferSubDataARB = 0;
PFNGLMAPBUFFERARBPROC			glMapBufferARB = 0;
PFNGLUNMAPBUFFERARBPROC			glUnmapBufferARB = 0;

//FRAME BUFFER EXTENTIONS
PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffersEXT = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC		glDeleteFramebuffersEXT = 0;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebufferEXT = 0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	glFramebufferTexture2DEXT = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatusEXT = 0;

//GLSL SHADER EXTENTIONS
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB = 0;
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB = 0;
PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB = 0;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB = 0;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB = 0;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB = 0;
PFNGLDETACHOBJECTARBPROC         glDetachObjectARB = 0;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB = 0;
PFNGLVALIDATEPROGRAMARBPROC      glValidateProgramARB = 0;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB = 0;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB = 0;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = 0;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB = 0;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB = 0;
PFNGLUNIFORM1IARBPROC            glUniform1iARB = 0;
PFNGLUNIFORM1IVARBPROC           glUniform1ivARB = 0;
PFNGLUNIFORM2IVARBPROC           glUniform2ivARB = 0;
PFNGLUNIFORM3IVARBPROC           glUniform3ivARB = 0;
PFNGLUNIFORM4IVARBPROC           glUniform4ivARB = 0;
PFNGLUNIFORM1FVARBPROC           glUniform1fvARB = 0;
PFNGLUNIFORM2FVARBPROC           glUniform2fvARB = 0;
PFNGLUNIFORM3FVARBPROC           glUniform3fvARB = 0;
PFNGLUNIFORM4FVARBPROC           glUniform4fvARB = 0;
PFNGLUNIFORMMATRIX2FVARBPROC     glUniformMatrix2fvARB = 0;
PFNGLUNIFORMMATRIX3FVARBPROC     glUniformMatrix3fvARB = 0;
PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fvARB = 0;
PFNGLBINDATTRIBLOCATIONARBPROC   glBindAttribLocationARB = 0;
#endif

#ifdef WIN32
 PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
#endif
#ifdef LINUX
 PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI = 0;
#endif


bool glInitExt(){
#ifndef ANDROID
  GLEXT_FN(glDrawRangeElements,		PFNGLDRAWRANGEELEMENTSPROC);
  GLEXT_FN(glActiveTextureARB,       PFNGLACTIVETEXTUREARBPROC);
  GLEXT_FN(glClientActiveTextureARB, PFNGLCLIENTACTIVETEXTUREARBPROC);

  GLEXT_FN(glGenBuffersARB, PFNGLGENBUFFERSARBPROC);
  GLEXT_FN(glDeleteBuffersARB, PFNGLDELETEBUFFERSARBPROC);
  GLEXT_FN(glBindBufferARB, PFNGLBINDBUFFERARBPROC);
  GLEXT_FN(glBufferDataARB, PFNGLBUFFERDATAARBPROC);
  GLEXT_FN(glBufferSubDataARB, PFNGLBUFFERSUBDATAARBPROC);
  GLEXT_FN(glGetBufferSubDataARB, PFNGLGETBUFFERSUBDATAARBPROC);
  GLEXT_FN(glMapBufferARB, PFNGLMAPBUFFERARBPROC);
  GLEXT_FN(glUnmapBufferARB, PFNGLUNMAPBUFFERARBPROC);

  GLEXT_FN(glGenFramebuffersEXT, PFNGLGENFRAMEBUFFERSEXTPROC);
  GLEXT_FN(glDeleteFramebuffersEXT, PFNGLDELETEFRAMEBUFFERSEXTPROC);
  GLEXT_FN(glBindFramebufferEXT, PFNGLBINDFRAMEBUFFEREXTPROC);
  GLEXT_FN(glFramebufferTexture2DEXT, PFNGLFRAMEBUFFERTEXTURE2DEXTPROC);
  GLEXT_FN(glCheckFramebufferStatusEXT, PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC);

  GLEXT_FN(glCreateProgramObjectARB, PFNGLCREATEPROGRAMOBJECTARBPROC);
  GLEXT_FN(glCreateShaderObjectARB,  PFNGLCREATESHADEROBJECTARBPROC);
  GLEXT_FN(glDeleteObjectARB,        PFNGLDELETEOBJECTARBPROC);
  GLEXT_FN(glShaderSourceARB,        PFNGLSHADERSOURCEARBPROC);
  GLEXT_FN(glCompileShaderARB,       PFNGLCOMPILESHADERARBPROC);
  GLEXT_FN(glAttachObjectARB,        PFNGLATTACHOBJECTARBPROC);
  GLEXT_FN(glDetachObjectARB,        PFNGLDETACHOBJECTARBPROC);
  GLEXT_FN(glLinkProgramARB,         PFNGLLINKPROGRAMARBPROC);
  GLEXT_FN(glValidateProgramARB,     PFNGLVALIDATEPROGRAMARBPROC);
  GLEXT_FN(glUseProgramObjectARB,    PFNGLUSEPROGRAMOBJECTARBPROC);
  GLEXT_FN(glGetObjectParameterfvARB,PFNGLGETOBJECTPARAMETERFVARBPROC);
  GLEXT_FN(glGetObjectParameterivARB,PFNGLGETOBJECTPARAMETERIVARBPROC);
  GLEXT_FN(glGetInfoLogARB,			PFNGLGETINFOLOGARBPROC);
  GLEXT_FN(glGetUniformLocationARB,	PFNGLGETUNIFORMLOCATIONARBPROC);
  GLEXT_FN(glUniform1iARB,			PFNGLUNIFORM1IARBPROC);
  GLEXT_FN(glUniform1ivARB,			PFNGLUNIFORM1IVARBPROC);
  GLEXT_FN(glUniform2ivARB,			PFNGLUNIFORM2IVARBPROC);
  GLEXT_FN(glUniform3ivARB,			PFNGLUNIFORM3IVARBPROC);
  GLEXT_FN(glUniform4ivARB,			PFNGLUNIFORM4IVARBPROC);
  GLEXT_FN(glUniform1fvARB,			PFNGLUNIFORM1FVARBPROC);
  GLEXT_FN(glUniform2fvARB,			PFNGLUNIFORM2FVARBPROC);
  GLEXT_FN(glUniform3fvARB,			PFNGLUNIFORM3FVARBPROC);
  GLEXT_FN(glUniform4fvARB,			PFNGLUNIFORM4FVARBPROC);
  GLEXT_FN(glUniformMatrix2fvARB,   PFNGLUNIFORMMATRIX2FVARBPROC);
  GLEXT_FN(glUniformMatrix3fvARB,   PFNGLUNIFORMMATRIX3FVARBPROC);
  GLEXT_FN(glUniformMatrix4fvARB,   PFNGLUNIFORMMATRIX4FVARBPROC);
  GLEXT_FN(glUniform4ivARB,         PFNGLUNIFORM4IVARBPROC);
  GLEXT_FN(glBindAttribLocationARB, PFNGLBINDATTRIBLOCATIONARBPROC);
#endif

#ifdef WIN32
 GLEXT_FN(wglSwapIntervalEXT,	PFNWGLSWAPINTERVALFARPROC);
#endif

#ifdef LINUX
 GLEXT_FN(glxSwapIntervalSGI,    PFNGLXSWAPINTERVALSGIPROC);
#endif

 return true;
}
