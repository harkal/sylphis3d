
uniform sampler2D currentRender;
uniform sampler2D dust;
uniform sampler2D tape;

varying float bright;

void main(void)
{  	
	vec4 color = texture2D(currentRender, gl_TexCoord[0].xy);
	vec4 dustColor = texture2D(dust, gl_TexCoord[1].xy);
	vec4 tapeColor = texture2D(tape, gl_TexCoord[2].xy);

      float monoColor = (color.x + color.y + color.z) * 0.33;

#if 0
      gl_FragColor.x = monoColor;
	gl_FragColor.y = monoColor;
	gl_FragColor.z = monoColor * 0.7;
#else
	gl_FragColor = color;
#endif

	gl_FragColor *= dustColor * tapeColor * bright;
}
