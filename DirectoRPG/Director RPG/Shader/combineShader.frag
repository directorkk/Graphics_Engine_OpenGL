uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 pixelSize;
uniform int type;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize;

	if( type == 0 )
		gl_FragColor = vec4( texture2D( texture1, pos ).rgb + texture2D( texture2, pos ).rgb, 1.0 );
	else if( type == 1 ){
		if( texture2D( texture2, pos ).rgb == vec3( 0.0, 0.0, 0.0 ) )
			gl_FragColor = texture2D( texture1, pos );
		else
			gl_FragColor = vec4( texture2D( texture1, pos ).rgb + texture2D( texture2, pos ).rgb, 1.0 );
	}
}