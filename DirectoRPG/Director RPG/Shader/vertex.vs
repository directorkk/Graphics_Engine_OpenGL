varying vec2 texcoord;
varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 position;

uniform vec3 lightPos;		// environment light
varying vec3 lightPosition;
uniform mat4 lightMatrix;


void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	position = vec3( gl_ModelViewMatrix * gl_Vertex );
	outNormal = gl_NormalMatrix * gl_Normal;
	outTangent = gl_NormalMatrix * outTangent;
	lightPosition = vec3( lightMatrix * vec4( lightPos, 1.0 ) );
	
	texcoord = gl_MultiTexCoord0.st;
}