
uniform sampler2DRect currentRender;
uniform sampler2DRect speedBuffer;

uniform float amount;

const int cNumSamples = 9;

void main(void)
{  
	vec4 color = vec4(0.0);
	
	vec2 vel = texture2DRect(speedBuffer, gl_TexCoord[0].xy).xy;

	float step = amount / float(cNumSamples * 2);
	vec2 tex = gl_TexCoord[0].xy;
	for(int i = 0 ; i < cNumSamples ; i++){
		vec2 lookup = tex + vel * step * float(i);        
      	vec4 col = texture2DRect(currentRender, lookup);
      	color += col;
	}
	for(int i = 1 ; i < cNumSamples ; i++){
		vec2 lookup = tex - vel * step * float(i);        
      	vec4 col = texture2DRect(currentRender, lookup);
      	color += col;
	}
	
	gl_FragColor = color / float(cNumSamples * 2 - 1);
	//gl_FragColor.rg = vel;
	//gl_FragColor.b = -vel.x;
}