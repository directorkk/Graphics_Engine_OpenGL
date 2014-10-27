varying vec3 position;
varying vec3 normal;
varying vec2 texcoord;

uniform mat4 modelMatrix;
uniform vec3 lightPos;
uniform mat4 lightMatrix;
uniform mat4 lightModelViewProjectionMatrix;

varying vec3 lightPosition;
varying vec3 tangentSurface2light;
varying vec3 tangentSurface2view;
varying vec4 lightVertexPosition;


void main()
{ 
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	lightVertexPosition = lightModelViewProjectionMatrix * ( modelMatrix * gl_Vertex );
	
	position = vec3( gl_ModelViewMatrix * gl_Vertex );
	normal = gl_NormalMatrix * gl_Normal;
	texcoord = gl_MultiTexCoord0.xy;
	lightPosition = vec3( lightMatrix * vec4( lightPos, 1.0 ) );

	vec3 tangent;
	vec3 v1 = cross( gl_Normal, vec3( 0.0, 0.0, -1.0 ) );
	vec3 v2 = cross( gl_Normal, vec3( 0.0, -1.0, 0.0 ) );

	if( length(v1) > length(v2) )
		tangent = v1;
	else
		tangent = v2;

	vec3 n = normalize( gl_NormalMatrix * gl_Normal );
	vec3 t = normalize( gl_NormalMatrix * tangent );
	vec3 b = cross( n, t );
	mat3 mat = mat3( t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z );		// the matrix which gets a vertex to a tangent-space

	vec3 vector = normalize( lightPosition - position );
	tangentSurface2light = mat * vector;	// surf2light in fragment
	vector = normalize( -position );
	tangentSurface2view = mat * vector;		// surf2camera in fragment.frag
}