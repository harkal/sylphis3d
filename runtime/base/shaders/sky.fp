
uniform float g;
uniform float g2;

varying vec3 lightDir;
varying vec3 direction;
varying vec4 color;
varying vec4 color2;

void main(void){
	float fCos = dot(lightDir, direction) / length(direction);
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);

#ifdef TEXCOORD_AS_POSITION
	gl_FragColor = color  + fMiePhase * color2;
#else
	gl_FragColor = compressHDR(color  + fMiePhase * color2);
#endif
}
