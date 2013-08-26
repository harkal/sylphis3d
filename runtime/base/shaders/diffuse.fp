
uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform sampler2D bumpMap;
uniform sampler3D attenuationMap;

varying	vec3 lightVec;
varying vec3 cameraVec;
varying	vec3 halfVec;
varying	vec3 attVec;

uniform vec2 bumpSize;
uniform vec4 lightColor;

#if 0
vec4 Texture2D(sampler2D colormap, vec2 coord){
	vec4 color = texture2D(colormap, coord);
	return color * color;
}
#else
#define Texture2D texture2D
#endif



void main()
{
	float lightAttenuation = texture3D(attenuationMap, attVec).r;
	
	vec3 nLightVec = normalize(lightVec);
	vec3 nCameraVec = normalize(cameraVec);
	
	float height = texture2D(bumpMap, gl_TexCoord[0].xy).r;
	height = height * bumpSize.x + bumpSize.y;

	vec2 newUV = gl_TexCoord[0].xy + nCameraVec.xy * height;
	vec4 color = Texture2D(colorMap, newUV.xy);
	vec3 bump = texture2D(normalMap, newUV.xy).rgb * 2.0 - 1.0;
	bump.y = -bump.y;
	
	float diffuse = max(dot(nLightVec, bump), 0.0);
	float specular = pow(max(dot(normalize(halfVec), bump), 0.0), 16.0);
	
	vec4 lighting = color * diffuse + specular;
	vec4 fcolor = lightColor * lighting * lightAttenuation;
	
	gl_FragColor = compressHDR(fcolor);
}

