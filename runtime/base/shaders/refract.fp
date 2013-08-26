
uniform sampler2D tex;
uniform sampler2D normal;
uniform vec2 renderNormalize;
uniform float amount;

void main(void)
{  
	vec2 coords = gl_FragCoord.xy * renderNormalize;
	
	vec2 n = texture2D(normal, coords).xy - 0.5;
	coords += n * amount;

      //gl_FragColor = texture2D(tex, clamp(coords, 0.0, 1.0));
	vec4 t = texture2D(tex, clamp(coords, 0.0, 1.0));
      //t.r = (t.r + t.b + t.g) / 3.0;
      //t.g = t.b = t.r;
	gl_FragColor = t;
}