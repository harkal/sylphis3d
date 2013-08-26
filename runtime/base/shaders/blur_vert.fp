
uniform sampler2D RT;

void main(void)
{
   vec2 vTexCoord = gl_TexCoord[0].xy;//gl_FragCoord.xy;
	
   //vTexCoord.x = vTexCoord.x / 800.0;
   vTexCoord.y = -vTexCoord.y;

   float d = 256.0;

   vec2 sam0 = vec2(0.0, 1.0) / d;
   vec2 sam1 = vec2(0.0, 2.0) / d;
   vec2 sam2 = vec2(0.0, -1.0) / d;
   vec2 sam3 = vec2(0.0, -2.0) / d;

   vec2 sam4 = vec2(0.0, 3.0) / d;
   vec2 sam5 = vec2(0.0, 4.0) / d;
   vec2 sam6 = vec2(0.0, -3.0) / d;
   vec2 sam7 = vec2(0.0, -4.0) / d;
	
   vec4 sum = texture2D(RT, vTexCoord);
   sum += texture2D(RT, vTexCoord + sam0);
   sum += texture2D(RT, vTexCoord + sam1);
   sum += texture2D(RT, vTexCoord + sam2);
   sum += texture2D(RT, vTexCoord + sam3);

   sum += texture2D(RT, vTexCoord + sam4);
   sum += texture2D(RT, vTexCoord + sam5);
   sum += texture2D(RT, vTexCoord + sam6);
   sum += texture2D(RT, vTexCoord + sam7);

   sum /= 9.0;

   gl_FragColor = sum;
}
