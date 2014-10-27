uniform vec2 pixelSize;
uniform sampler2D texture;
uniform int isVertical;
uniform int type;

void main()
{
	vec2 pos = gl_FragCoord.xy * pixelSize;
	vec4 result = vec4( 0.0, 0.0, 0.0, 0.0 );

	if( type == 0 ){
		// blur 1
		// to caculate the color of the central one
		// mix the pixel all around by their single weight
		// 1 2 1
		// 2 3 2
		// 1 2 1
		result += 1.0 * texture2D( texture, vec2( pos.x-pixelSize.x, pos.y-pixelSize ) );
		result += 2.0 * texture2D( texture, vec2( pos.x, pos.y-pixelSize ) );
		result += 1.0 * texture2D( texture, vec2( pos.x+pixelSize.x, pos.y-pixelSize ) );
		result += 2.0 * texture2D( texture, vec2( pos.x-pixelSize.x, pos.y ) );
		result += 3.0 * texture2D( texture, vec2( pos.x, pos.y ) );
		result += 2.0 * texture2D( texture, vec2( pos.x+pixelSize.x, pos.y ) );
		result += 1.0 * texture2D( texture, vec2( pos.x-pixelSize.x, pos.y+pixelSize ) );
		result += 2.0 * texture2D( texture, vec2( pos.x, pos.y+pixelSize ) );
		result += 1.0 * texture2D( texture, vec2( pos.x+pixelSize.x, pos.y+pixelSize ) );
		result /= 15.0;
	}

	if( type == 1 ){
		// blur 2
		float value[9];
		value[0] = 0.05;
		value[1] = 0.09;
		value[2] = 0.11;
		value[3] = 0.15;
		value[4] = 0.2;
		value[5] = 0.15;
		value[6] = 0.11;
		value[7] = 0.09;
		value[8] = 0.05;

		if( isVertical == 1 ){
			vec2 curPos = vec2( pos.x, pos.y-4.0*pixelSize.y );
			for( int i=0 ; i<9 ; i++ ){
				result += texture2D( texture, curPos )*value[i];
				curPos.y += pixelSize.y;
			}
		}
		else{
			vec2 curPos = vec2( pos.x-4.0*pixelSize.x, pos.y );
			for( int i=0 ; i<9 ; i++ ){
				result += texture2D( texture, curPos )*value[i];
				curPos.x += pixelSize.x;
			}
		}
	}

	gl_FragColor = vec4( result.rgb, 1.0 );
}