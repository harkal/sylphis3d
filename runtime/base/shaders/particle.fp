
uniform sampler2D ambientTexture;
uniform	vec3 color;

void main(void){	
	gl_FragColor.rgb = compressHDR(texture2D(ambientTexture, gl_TexCoord[0].xy).rgb * color);
//	gl_FragColor.rgb = texture2D(ambientTexture, gl_TexCoord[0].xy).rgb * color;
}
