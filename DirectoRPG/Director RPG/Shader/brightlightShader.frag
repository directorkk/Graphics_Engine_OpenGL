uniform vec2 pixelSize;
uniform sampler2D texture;

uniform float glowRate;		// 0.3 .. 0.697

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize;
	float intensity = dot( texture2D( texture, pos ).rgb ,vec3(0.1, 0.1, 0.8) );

	if( intensity > 0.8 )
		gl_FragColor = vec4( 0.7, 0.7, 0.7, 1.0 );
	else
		gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
}