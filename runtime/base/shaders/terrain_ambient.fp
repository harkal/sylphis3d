
uniform sampler2D colorMap;
uniform sampler2D grassMap;
uniform sampler2D rockMap;

uniform vec4 lightColor;

void main()
{	
	vec4 color = texture2D(colorMap, gl_TexCoord[0].xy);
	vec4 grass = texture2D(grassMap, gl_TexCoord[1].xy);
	vec4 rock = texture2D(rockMap, gl_TexCoord[1].xy);
		
	float l = min(color.g, 0.7) * 1.42;

	vec4 lighting = color * (rock * (1.0 - l) + grass * l);
	vec4 fcolor = lightColor * lighting;

	gl_FragColor = compressHDR(fcolor);
}

