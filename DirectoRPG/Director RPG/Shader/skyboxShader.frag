varying vec3 vertixVector;

uniform samplerCube cubeMap;

void main()
{	
	gl_FragColor = textureCube( cubeMap, vertixVector );
}