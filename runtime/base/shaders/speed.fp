
uniform mat4 lastMVPMatrix;
varying vec4 pos;

void main(void)
{
	vec4 newPos = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;
	vec4 lastPos = gl_ProjectionMatrix * lastMVPMatrix * pos;
      
	newPos /= newPos.w;
	lastPos /= lastPos.w;
	vec2 velocity = newPos.xy - lastPos.xy;
    
	gl_FragColor.rgb = vec3(velocity.x, velocity.y, 0.0);
}