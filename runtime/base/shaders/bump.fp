
uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform sampler2D glossMap;
uniform sampler3D attenuationMap;

varying	vec3 lightVec;
varying vec3 cameraVec;
varying	vec3 halfVec;
varying	vec3 attVec;

uniform vec4 lightColor;

vec4 Texture2D(sampler2D colormap, vec2 coord){
	vec4 color = texture2D(colormap, coord);
	return color * color;
}

void main()
{
	float lightAttenuation = texture3D(attenuationMap, attVec).r;
	
	vec3 nLightVec = normalize(lightVec);
	vec3 nCameraVec = normalize(cameraVec);
	
	vec4 gloss = texture2D(glossMap, gl_TexCoord[0].xy);
	vec4 color = texture2D(colorMap, gl_TexCoord[0].xy);
	vec3 bump = texture2D(normalMap, gl_TexCoord[0].xy).rgb * 2.0 - 1.0;
	bump.y = -bump.y;
	
	float diffuse = max(dot(nLightVec, bump), 0.0);
	float specular = pow(max(dot(normalize(halfVec), bump), 0.0), 16.0);
	
	vec4 lighting = color * diffuse + gloss * specular;
	vec4 fcolor = lightColor * lighting * lightAttenuation;

	gl_FragColor = compressHDR(fcolor);
}

