
varying vec2 texCoord;

uniform sampler2D RT;
uniform float texSize;
uniform vec3 limits;

void main(void)
{
	vec3 luma = texture2D(RT, texCoord).rgb;
	luma += texture2D(RT, texCoord - vec2(texSize, 0.0)).rgb;
	luma += texture2D(RT, texCoord - vec2(0.0, texSize)).rgb;
	luma += texture2D(RT, texCoord - vec2(texSize, texSize)).rgb;
	luma *= 0.25;
	
	float luminance = min(max(luma.r, limits.x), limits.y);
	float expos = 0.22  / (luminance + 0.001);
	
	gl_FragColor.r = expos;
}
