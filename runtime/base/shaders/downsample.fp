
varying vec2 texCoord;

uniform sampler2D RT;
uniform float texSize;

void main(void)
{
	vec3 color = texture2D(RT, texCoord).rgb;
	color += texture2D(RT, texCoord - vec2(texSize, 0.0)).rgb;
	color += texture2D(RT, texCoord - vec2(0.0, texSize)).rgb;
	color += texture2D(RT, texCoord - vec2(texSize, texSize)).rgb;
   
	gl_FragColor.rgb = color * 0.25;
}