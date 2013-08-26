
uniform sampler2D ambientTexture;
uniform float exposure;

vec3 compressHDR(vec3 color){
	//return 1.0 - exp(-exposure * color);
	return exposure * color;
}


void main(void){
	//gl_FragColor.rgb = compressHDR(texture2D(ambientTexture, gl_TexCoord[0].xy).rgb);
	gl_FragColor.rgb = texture2D(ambientTexture, gl_TexCoord[0].xy).rgb;
}
