uniform mat4   g_mView;
uniform mat4   g_mProj;
uniform mat4   g_mTran;

attribute vec3 g_Vertex;
attribute vec4 g_Color;
attribute vec2 g_Texcoord;

varying   vec4 vColor;
varying   vec2 Texcoord;

void main()
{
   gl_Position  = g_mProj * g_mView * g_mTran * vec4(g_Vertex, 1.0);
   vColor       = g_Color;
   Texcoord     = g_Texcoord;
}

//Fragment Shader
#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D g_txColor;

varying vec4 vColor;
varying vec2 Texcoord;

void main()
{
  vec4 color   = texture2D(g_txColor, Texcoord);
  gl_FragColor = vColor * color;
}
