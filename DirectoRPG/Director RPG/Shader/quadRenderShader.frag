varying vec2 texcoord;

uniform vec2 pixelSize;	// 1.0/640.0, 1.0/480.0
uniform sampler2D tex;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize;

	// ¦Ç¶¥
	// intensity = (r+g+b)/3 = dot( color, vec3(0.3333, 0.3333, 0.3333) )
	// float intensity = dot( texture2D( texture, outUV ).rgb ,vec3(0.3333, 0.3333, 0.3333) );

	gl_FragColor = texture2D( tex, texcoord );
	//gl_FragColor = vec4( 0.0, 0.0, 1.0, 1.0 );
}