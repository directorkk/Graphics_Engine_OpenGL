#ifndef _DRPG_OPENGL_MESH_
#define _DRPG_OPENGL_MESH_

#include "drpg_header.h"
#include "drpg_glModelData.h"
#include "drpg_glTextureSource.h"

namespace drpg
{
	enum DRM_TextureMode{
		DRM_NORMALMAPPING = 0,
		DRM_ONETEXTURE,
		DRM_MULTITEXTURE,
		DRM_CUBEMAPPING
	};

	class drMesh
	{
		std::vector<vertexData> data;
		std::vector<unsigned int> indices;
		std::vector<textureData> tex;
		unsigned int VBO;
		unsigned int IND;
	public:
		drMesh( 
			std::vector<vertexData>* vd,
			std::vector<unsigned int>* id,
			std::vector<textureData>* td = NULL	);
		~drMesh();

		void draw( unsigned int programID, int textureMode, drTextureSource* textures, drTextureSource* normalTextures = NULL );
	};
}

#endif