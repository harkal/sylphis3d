
uniform sampler2D colorMap;
//uniform sampler2D glossMap;
uniform sampler2D detailMap;
uniform sampler2D grassMap;
uniform sampler2D rockMap;
uniform sampler3D attenuationMap;

varying	vec3 lightVec;
varying	vec3 attVec;

uniform vec4 lightColor;

void main()
{
	float lightAttenuation = texture3D(attenuationMap, attVec).r;
	
	vec3 nLightVec = normalize(lightVec);

	//vec4 gloss = texture2D(glossMap, gl_TexCoord[0].xy);
	vec4 color = texture2D(colorMap, gl_TexCoord[0].xy);
	vec4 grass = texture2D(grassMap, gl_TexCoord[1].xy);
	vec4 rock = texture2D(rockMap, gl_TexCoord[1].xy);
	vec3 bump = texture2D(detailMap, gl_TexCoord[1].xy).rgb * 2.0 - 1.0;
	
	float diffuse = max(dot(nLightVec, bump), 0.0);
	
	float l = min(color.g, 0.7) * 1.42;

	vec4 lighting = (color * (rock * (1.0 - l) + grass * l)) * diffuse;
	vec4 fcolor = lightColor * lighting * lightAttenuation;

	gl_FragColor = compressHDR(fcolor);
}

