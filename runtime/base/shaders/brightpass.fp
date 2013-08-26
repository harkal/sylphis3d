
uniform sampler2D ambientTexture;


const float BRIGHT_PASS_THRESHOLD = 0.8;
const float BRIGHT_PASS_OFFSET = 0.01;


void main(void){
	vec3 color = texture2D(ambientTexture, gl_TexCoord[0].xy).rgb;
	
	color = max(color - BRIGHT_PASS_THRESHOLD, 0.0);
	
	gl_FragColor.rgb = color / (color + BRIGHT_PASS_OFFSET);
}
