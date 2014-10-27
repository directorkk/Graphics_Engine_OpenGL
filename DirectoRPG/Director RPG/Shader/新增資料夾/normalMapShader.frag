uniform sampler2D tex;
uniform sampler2D normaltex;
uniform sampler2DShadow shadowMap;
uniform int shadowMode;
varying vec2 texcoord;

varying vec3 position;
varying vec3 normal;

uniform vec3 lightPos;
varying vec3 lightPosition;

uniform vec3 mambient;	//gl_FrontMaterial.ambient
uniform vec3 mdiffuse;	//				  .diffuse
uniform vec3 mspecular; //				  .specular

uniform float shininess;//				  .shininess

						//				 ↓ which light
uniform vec3 lambient;	//gl_LightSource[0].ambient
uniform vec3 ldiffuse;	//				   .diffuse
uniform vec3 lspecular;	//				   .specular

varying vec3 tangentSurface2light;
varying vec3 tangentSurface2view;
varying vec4 lightVertexPosition;

void main()
{
	float shadowValue = 0.0;
	/*
	if( shadow2DProj( shadowMap, lightVertexPosition ).r > 0.7 )
		shadowValue = 1.0;
	else
		shadowValue = 0.0;
	*/
	// 勻化影子, 透過偵測影子邊緣(沒有影子的部分), 周圍假如有影子就加暗一點
	// 類似Blur的做法, 跑愈多次愈細緻
	// x, y範圍愈大, 勻化愈多
	
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

	//float dist = length( position - lightPosition );
	//float att = 1.0/( 1.0 + 0.0001*dist + 0.00001*dist*dist );

	// 環境光
	vec3 ambient = texcolor * mambient * lambient;

	// 散射光
	vec3 surf2light = normalize( tangentSurface2light );
	//vec3 orinorm = normalize( normal );
	vec3 norm = normalize( texture2D( normaltex, texcoord ).xyz*2.0 - 1.0 );
	float dcont = max( 0.0, dot( norm, surf2light) );	// 只留下大於0的(角度小於90度), 小於0都視為0(角度大於等於90度)
	vec3 diffuse;
	if( shadowMode == 1 )
		diffuse = ( shadowValue ) * dcont * texcolor * mdiffuse * ldiffuse;
	else
		diffuse = dcont * texcolor * mdiffuse * ldiffuse;

	// 反射光
	vec3 surf2view = normalize( tangentSurface2view );
	vec3 reflection = reflect( -surf2light, norm );
	float scont = pow( max( 0.0, dot( surf2view, reflection ) ), shininess );
	vec3 specular;
	if( shadowMode == 1 )
		specular = shadowValue < 1 ? vec3( 0.0, 0.0, 0.0 ) : scont * mspecular * lspecular;
	else
		specular = scont * mspecular * lspecular;

	gl_FragColor = vec4( ( ambient + diffuse + specular ), 1.0 );
	//gl_FragColor = vec4( shadowValue, shadowValue, shadowValue, 1.0 );
}