
uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform sampler2D bumpMap;
uniform sampler3D attenuationMap;

varying	vec3 lightVec;
varying     vec3 cameraVec;
varying	vec3 halfVec;
varying	vec3 attVec;

uniform vec2 bumpSize;
uniform vec4 lightColor;

void main()
{
	float lightAttenuation = texture3D(attenuationMap, attVec).r;
	
	vec3 nLightVec = normalize(lightVec);
	
	float height = texture2D(bumpMap, gl_TexCoord[0].xy).r;
	height = height * bumpSize.x + bumpSize.y;

	vec2 newUV;
	newUV.x = gl_TexCoord[0].x + cameraVec.x * height;
	newUV.y = gl_TexCoord[0].y - cameraVec.y * height;
	vec4 color = texture2D(colorMap, newUV.xy);
	vec3 bump = texture2D(normalMap, newUV.xy).rgb * 2.0 - 1.0;

	float base = texture2D(bumpMap, newUV.xy).r;
	
	float diffuse = max(dot(nLightVec, bump), 0.0);
	float specular = pow(max(dot(normalize(halfVec), bump), 0.0), 16.0);
	
	gl_FragColor = color * lightColor * (diffuse * base + specular) * lightAttenuation;
}

