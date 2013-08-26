
uniform sampler2D RT;

varying vec2 texCoord;

const float texDimension = 256.0;
const float texScaler =  1.0/texDimension;
const float texOffset = -0.5/texDimension;

void main(void)
{
   vec4 color = vec4(0.0,0.0,0.0,0.0);
   
   const float gauss0 = 2.0/36.0;
   const float gauss1 = 4.0/36.0;
   const float gauss2 =8.0/36.0;
   const float gauss3 =8.0/36.0;
   const float gauss4 =8.0/36.0;
   const float gauss5 = 4.0/36.0;
   const float gauss6 = 2.0/36.0;   
   
   vec4 gaussFilter[7];
   gaussFilter[0]  = vec4( -3.0*texScaler , 0.0, 0.0, gauss0); 
   gaussFilter[1]  = vec4( -2.0*texScaler , 0.0, 0.0, gauss1); 
   gaussFilter[2]  = vec4( -1.0*texScaler , 0.0, 0.0, gauss2); 
   gaussFilter[3]  = vec4(  0.0*texScaler , 0.0, 0.0, gauss3);
   gaussFilter[4]  = vec4( +1.0*texScaler , 0.0, 0.0, gauss4);
   gaussFilter[5]  = vec4( +2.0*texScaler , 0.0, 0.0, gauss5);
   gaussFilter[6]  = vec4( +3.0*texScaler , 0.0, 0.0, gauss6);
  
   int i;
   for (i=0;i<7;i++)
      color += texture2D(RT, texCoord + gaussFilter[i].xy) * gaussFilter[i].w;
   
   gl_FragColor = color;
}
