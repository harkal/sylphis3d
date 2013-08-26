
uniform sampler2D RT;
varying vec2 texCoord;

const vec3 LUMINANCE_VECTOR = vec3(0.2125, 0.7154, 0.0721);

void main(void)
{
	vec4 color = decompressHDR(texture2D(RT, gl_TexCoord[0].xy));

	float luma = dot(color.rgb, LUMINANCE_VECTOR) + 0.0001;
		
	gl_FragColor.rgb = vec3(luma, luma, luma);
}


