uniform mat4 g_mLight;

varying vec4 q;

void kgm_main(out vec4 pos)
{
  q = g_mLight * g_mTran * vec4(a_Vertex, 1);

  vec3 v = a_Vertex;

  //v.z += 0.1;

  pos = g_mProj * g_mView * g_mTran * vec4(v, 1);
}

//Fragment Shader
uniform sampler2DShadow txDepth;
//uniform sampler2D txDepth;

varying vec4 q;

void kgm_main(out vec4 col)
{
  //vec3 scoord = 0.5 * (q.xyz / q.w) + 0.5;
  vec3 scoord = q.xyz / q.w;
  scoord.xy = 0.5 * scoord.xy + 0.5;

  /*scoord.xy = clamp(scoord.xy, 0.01, 0.99);

  if((scoord.x < 0.0) || (scoord.x > 1.0))
    discard;

  if((scoord.y < 0.0) || (scoord.y > 1.0))
    discard;

  scoord = clamp(scoord, 0.0, 1.0);*/

  //float shadow = 1.0;
  scoord.z += 0.0002f;
  float shadow = shadow2D(txDepth, scoord).r;
  //float shadow = texture2D(txDepth, scoord.xy).r;
  float depth  = scoord.z;

  //if (shadow >= depth)
  //    discard;

  //if(shadow > 0.99 || shadow < 0.01)
  //  discard;

  //if (shadow >= depth)
  //  discard;

  float d = depth - shadow;
  //d = clamp(d, 0.0, 1.0);
  //vec3 color = vec3(depth, depth, depth);
  //vec3 color = vec3(shadow, shadow, shadow);
  vec3 color = vec3(d, d, d);

  //if (d < 0.000001)
  //  discard;

  //color *= shadow;
  col = vec4(color.xyz, 0.5);
}
