varying vec3 vertixVector;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	vertixVector = gl_Vertex.xyz;
}