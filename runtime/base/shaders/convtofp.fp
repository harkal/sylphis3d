
uniform sampler2D RT;
varying vec2 texCoord;

const float BRIGHT_PASS_THRESHOLD = 0.75;
const float BRIGHT_PASS_OFFSET = 0.25;

const vec3 LUMINANCE_VECTOR = vec3(0.2125, 0.7154, 0.0721);

void main(void)
{
	vec4 color = texture2D(RT, gl_TexCoord[0].xy);

	color -= BRIGHT_PASS_THRESHOLD;
	
	color = max(color, 0.0);
	
	color.rgb /= BRIGHT_PASS_OFFSET + color.rgb;
	//color = compressHDR(color);

	gl_FragColor = color;
}


