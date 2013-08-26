
uniform sampler2D bloom256;
uniform sampler2D bloom128;
uniform sampler2D bloom64;
uniform sampler2D bloom32;
uniform sampler2D bloom16;

const float BRIGHT_PASS_THRESHOLD = 0.8;
const float BRIGHT_PASS_OFFSET = 0.01;

void main(void){
	vec3 s1 = texture2D(bloom256, gl_TexCoord[0].xy).rgb;
	vec3 s2 = texture2D(bloom128, gl_TexCoord[0].xy).rgb;
	vec3 s3 = texture2D(bloom64, gl_TexCoord[0].xy).rgb;
	vec3 s4 = texture2D(bloom32, gl_TexCoord[0].xy).rgb;
	vec3 s5 = texture2D(bloom16, gl_TexCoord[0].xy).rgb;
		
	//gl_FragColor.rgb = color / (color + BRIGHT_PASS_OFFSET);
	vec3 bloom = (s1 + s2 + s3 + s4 + s5)*0.2;
	gl_FragColor.rgb = bloom;
}
