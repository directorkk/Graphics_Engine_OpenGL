#include "drpg_glMesh.h"
using namespace drpg;


drMesh::drMesh( 
	std::vector<vertexData>* vd,
	std::vector<unsigned int>* id,
	std::vector<textureData>* td )
{
	data = *vd;
	indices = *id;
	tex = *td;

	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, data.size()*sizeof(vertexData), &data[0], GL_STATIC_DRAW );
	// GL_STREAM_DRAW: when vertices data could be updated between each rendering.
	// GL_DYNAMIC_DRAW: when vertices data could be updated between each frames.
	// GL_STATIC_DRAW: when vertices data are never or almost never updated.

	glGenBuffers( 1, &IND );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IND );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

drMesh::~drMesh()
{
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &IND );
}


void drMesh::draw( unsigned int programID, int textureMode, drTextureSource* textures, drTextureSource* normalTextures )
{
	int vertex = glGetAttribLocation( programID, "vertex" ),
		normal = glGetAttribLocation( programID, "normal" ),
		tangent = glGetAttribLocation( programID, "tangent" ),
		color = glGetAttribLocation( programID, "color" ),
		UV = glGetAttribLocation( programID, "UV" );

	if( textureMode == DRM_ONETEXTURE ){
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D,textures->textureID[tex[0].id]);			
		glUniform1i( glGetUniformLocation( programID, "tex" ), 0 );
	}
	else if( textureMode == DRM_NORMALMAPPING ){
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D,textures->textureID[tex[0].id]);			
		glUniform1i( glGetUniformLocation( programID, "tex" ), 0 );
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture(GL_TEXTURE_2D,normalTextures->textureID[tex[0].id]);			
		glUniform1i( glGetUniformLocation( programID, "normaltex" ), 1 );
	}
	else if( textureMode == DRM_CUBEMAPPING ){						
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_CUBE_MAP, textures->textureID[0] );
		glUniform1i( glGetUniformLocation( programID, "cubeMap" ), 0 );
	}

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IND );

	glEnableVertexAttribArray( vertex );
	glVertexAttribPointer( vertex, 3, GL_DOUBLE, GL_FALSE, sizeof(vertexData), 0 );
	
	glEnableVertexAttribArray( normal );
	glVertexAttribPointer( normal, 3, GL_DOUBLE, GL_FALSE, sizeof(vertexData), (void*)(3*sizeof(double)) );

	glEnableVertexAttribArray( tangent );
	glVertexAttribPointer( tangent, 3, GL_DOUBLE, GL_FALSE, sizeof(vertexData), (void*)(6*sizeof(double)) );

	glEnableVertexAttribArray( color );
	glVertexAttribPointer( color, 3, GL_DOUBLE, GL_FALSE, sizeof(vertexData), (void*)(9*sizeof(double)) );

	glEnableVertexAttribArray( UV );
	glVertexAttribPointer( UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(12*sizeof(double)) );

	glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );

	glDisableVertexAttribArray( vertex );
	glDisableVertexAttribArray( normal );
	glDisableVertexAttribArray( tangent );
	glDisableVertexAttribArray( color );
	glDisableVertexAttribArray( UV );
	
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}
