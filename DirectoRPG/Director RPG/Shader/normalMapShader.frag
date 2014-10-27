uniform sampler2D tex;
uniform sampler2D normaltex;
uniform sampler2DShadow shadowMap;
uniform int shadowMode;

varying vec2 texcoord;
varying vec3 position;
varying vec3 normal;

	
uniform float shininess;//				  .shininess
//
//
// * * * * * * environment light * * * * * * //
	uniform vec3 lightPos;
	varying vec3 lightPosition;

	uniform vec3 mambient;	//gl_FrontMaterial.ambient
	uniform vec3 mdiffuse;	//				  .diffuse
	uniform vec3 mspecular; //				  .specular

							//				 ↓ which light
	uniform vec3 lambient;	//gl_LightSource[0].ambient
	uniform vec3 ldiffuse;	//				   .diffuse
	uniform vec3 lspecular;	//				   .specular
// * * * * * * info end * * * * * * //
//
//
//
// * * * * * * normal light * * * * * * //
	uniform float MAX_DISTANCE;
	uniform int AMOUNT_LIGHT;
	uniform vec4 lightWorldSite[10];	// modelViewMatrix * lightSite
	varying vec3 tangentSurf2nLight;

	uniform vec3 nlambient[10];
	uniform vec3 nldiffuse[10];
	uniform vec3 nlspecular[10];

	uniform vec3 nmambient[10];
	uniform vec3 nmdiffuse[10];
	uniform vec3 nmspecular[10];
// * * * * * * info end * * * * * * //
//
//
varying vec3 tangentSurface2light;
varying vec3 tangentSurface2view;
varying vec4 lightVertexPosition;

vec3 mixBright( vec3 aColor, vec3 bColor )
{
	vec3 tmp;
	if( aColor.r >= bColor.r )
		tmp.r = aColor.r;
	else
		tmp.r = bColor.r;

	if( aColor.g >= bColor.g )
		tmp.g = aColor.g;
	else
		tmp.g = bColor.g;

	if( aColor.b >= bColor.b )
		tmp.b = aColor.b;
	else
		tmp.b = bColor.b;

	return tmp;
}

void main()
{
	float shadowValue = 0.0;

	if( shadowMode == 1 ){
		for( float x=-0.05 ; x<=0.05 ; x+=0.02 ){
			for( float y=-0.05 ; y<=0.05 ; y+=0.02 ){
				if( shadow2DProj( shadowMap, lightVertexPosition + vec4( x, y, 0, 0 ) ).r == 1.0 )
					shadowValue += 1.0;
			}
		}
		shadowValue /= 25.0;
	}
	
	vec3 texcolor = vec3( texture2D( tex, texcoord ) );

	// * * * * * * environment light * * * * * * //
		vec3 surf2light = normalize( tangentSurface2light );
		vec3 norm = normalize( texture2D( normaltex, texcoord ).xyz*2.0 - 1.0 );
		vec3 surf2view = normalize( tangentSurface2view );
		vec3 reflection = reflect( -surf2light, norm );

		float dcont = max( 0.0, dot( norm, surf2light) );	// 只留下大於0的(角度小於90度), 小於0都視為0(角度大於等於90度)
		float scont = pow( max( 0.0, dot( surf2view, reflection ) ), shininess );
	// * * * * * * light info * * * * * * //

	vec3 finalLambient = lambient;
	vec3 finalLdiffuse = ldiffuse;
	vec3 finalLspecular = lspecular;

	vec3 finalMambient = mambient;
	vec3 finalMdiffuse = mdiffuse;
	vec3 finalMspecular = mspecular;

	int finalShadowNode = shadowMode;
	
	// * * * * * * normal light * * * * * * //
		vec3 surf2nLight;
		vec3 nLightReflection;
		float countLight = 0.0;
		for( int i=0 ; i<AMOUNT_LIGHT ; i++ ){
			float tmpLen = length( lightWorldSite[i].xyz - position );
			float lenCont;
			if( tmpLen > MAX_DISTANCE || tmpLen == 0.0 ){
				lenCont = 0.0;
				finalShadowNode = 1;
			}
			else{
				float tmpRate;
				if( tmpLen > MAX_DISTANCE-0.5 ){
					tmpRate = 0.1;
				}
				else if( tmpLen > MAX_DISTANCE-1.0 ){
					tmpRate = 0.5;
				}
				else if( tmpLen > MAX_DISTANCE-1.5 ){
					tmpRate = 1.0;
				}
				else if( tmpLen > MAX_DISTANCE-2.0 ){
					tmpRate = 2.0;
				}
				else if( tmpLen > MAX_DISTANCE-2.5 ){
					tmpRate = 2.5;
				}
				else{
					tmpRate = 3.0;
				}
				lenCont = 1.0/(tmpLen)*tmpRate;
				countLight++;
				
				finalLambient = nlambient[i];
				finalLdiffuse = nldiffuse[i];
				finalMambient = nmambient[i];
				finalMdiffuse = nmdiffuse[i];
				
				finalShadowNode = 0;
			}
		}
	// * * * * * * light info * * * * * * //

	//finalMspecular /= countLight+1.0;
	//finalLspecular /= countLight+1.0;

	
	
	// * * * * * * 環境光 * * * * * * //
	vec3 ambient = texcolor * finalMambient * finalLambient;

	// * * * * * * 散射光 * * * * * * //
	vec3 diffuse;
	if( finalShadowNode == 1 )
		diffuse = ( shadowValue ) * dcont * texcolor * finalMdiffuse * finalLdiffuse;
	else
		diffuse = dcont * texcolor * finalMdiffuse * finalLdiffuse;

	// * * * * * * 反射光 * * * * * * //
	vec3 specular;
	if( finalShadowNode == 1 )
		specular = shadowValue < 1.0 ? vec3( 0.0, 0.0, 0.0 ) : scont * finalMspecular * finalLspecular;
	else
		specular = scont * finalMspecular * finalLspecular;
	


	gl_FragColor = vec4( ( ambient + diffuse + specular ), 1.0 );
}