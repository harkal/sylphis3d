
uniform sampler2D RT;

varying vec2 texCoord;
uniform float texSize;

void main(void)
{
   vec4 color = vec4(0.0,0.0,0.0,0.0);
   
   const float gauss0 = 1.0/32.0;
   const float gauss1 =10.0/32.0;
   const float gauss2 =20.0/32.0;
   const float gauss3 =26.0/32.0;
   const float gauss4 =20.0/32.0;
   const float gauss5 =10.0/32.0;
   const float gauss6 = 1.0/32.0;     
   
   vec4 gaussFilter[7];
   gaussFilter[0]  = vec4( -3.0*texSize , 0.0, 0.0, gauss0).yxzw;
   gaussFilter[1]  = vec4( -2.0*texSize , 0.0, 0.0, gauss1).yxzw;
   gaussFilter[2]  = vec4( -1.0*texSize , 0.0, 0.0, gauss2).yxzw;
   gaussFilter[3]  = vec4(  0.0*texSize , 0.0, 0.0, gauss3).yxzw;
   gaussFilter[4]  = vec4( +1.0*texSize , 0.0, 0.0, gauss4).yxzw;
   gaussFilter[5]  = vec4( +2.0*texSize , 0.0, 0.0, gauss5).yxzw;
   gaussFilter[6]  = vec4( +3.0*texSize , 0.0, 0.0, gauss6).yxzw;
  
   int i;
   for (i=0;i<7;i++)
      color += texture2D(RT, texCoord + gaussFilter[i].xy) * gaussFilter[i].w;
   
   gl_FragColor = (color);
}