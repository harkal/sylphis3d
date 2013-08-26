
uniform sampler2D RT;
varying vec2 texCoord;

const float BRIGHT_PASS_THRESHOLD = 0.0;

void main(void)
{
	vec3 color = texture2D(RT, gl_TexCoord[0].xy).rgb;

	gl_FragColor.rgb = compressHDR(color);
}


