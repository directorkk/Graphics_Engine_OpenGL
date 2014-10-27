uniform sampler2D tex;
uniform int AMOUNT_LIGHT;
uniform vec4 lightWorldSite[100];	// modelViewMatrix * lightSite

varying vec2 texcoord;
varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 position;
varying vec3 lightPosition;

const vec4 ambientColor = vec4( 0.5, 0.5, 0.5, 1.0 );
const vec4 diffuseColor = vec4( 0.7, 0.7, 0.7, 1.0 );
const vec4 specularColor = vec4( 0.1, 0.1, 0.1, 1.0 );

void main()
{
	vec4 texcolor = texture2D( tex, texcoord );

	vec3 surf2lightP[100];
	for( int i=0 ; i<AMOUNT_LIGHT ; i++ ){
		surf2lightP[i] = normalize( lightWorldSite[i].xyz - position );
	}
	
	vec3 normal = normalize( outNormal );
	vec3 surf2light = normalize( lightPosition - position );
	vec3 surf2camera = normalize( -position );
	vec3 reflection = -reflect( surf2camera, normal );

	vec3 ambient = vec3( ambientColor * texcolor );

	float diffuseCont[100];
	for( int i=0 ; i<AMOUNT_LIGHT ; i++ ){
		diffuseCont[i] = max( 0.0, dot( normal, surf2lightP[i] ) );
	}
	float diffuseContribution = max( 0.0, dot( normal, surf2light ) );
	vec3 diffuse = vec3( diffuseContribution * diffuseColor * texcolor );
	for( int i=0 ; i<AMOUNT_LIGHT ; i++ ){
		diffuse += vec3( diffuseCont[i] * diffuseColor * texcolor );
	}
	diffuse /= float(AMOUNT_LIGHT + 1);
	
	float specularCont[100];
	for( int i=0 ; i<AMOUNT_LIGHT ; i++ ){
		specularCont[i] = pow( max( 0.0, dot( reflection, surf2lightP[i] ) ), 32.0 );
	}
	float specularContribution = pow( max( 0.0, dot( reflection, surf2light ) ), 32.0);
	vec3 specular = vec3( specularContribution * specularColor );
	for( int i=0 ; i<AMOUNT_LIGHT ; i++ ){
		specular += vec3( specularCont[i] * specularColor );
	}
	specular /= float(AMOUNT_LIGHT + 1);
	
	gl_FragColor = vec4( ambient + diffuse + specular, 1.0 );
	//gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}