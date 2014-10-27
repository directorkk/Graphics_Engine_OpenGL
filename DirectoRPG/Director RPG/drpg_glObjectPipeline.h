#ifndef _DRPG_OPENGL_OBJECT_PIPELINE_
#define _DRPG_OPENGL_OBJECT_PIPELINE_

#include "drpg_header.h"
#include "drpg_glModel.h"



namespace drpg
{
	enum updateType{
		DRO_ROTATE = 0,
		DRO_TRANSLATE,
		DRO_SCALE
	};

	class drObjectPipeline
	{
		std::deque<drModel> objectlist;
		std::deque<std::string> namelist;
		
	public:
		drObjectPipeline();
		~drObjectPipeline();
		
		void draw( unsigned int programID, int textureMode, drTextureSource* textures, drTextureSource* normalTextures = NULL );

		bool addObject( drModel obj, const char* objName );
		bool delObject( const char* objName );
		bool updateObject( drModel obj, const char* objName );
		bool updateObject( int updateType, const char* objName, double updateX, double updateY, double updateZ, bool addToNow = false );
		bool updateObject( int updateType, const char* objName, vector3 updateVec, bool addToNow = false );

		vector3 getState( int stateType, const char* objName );

		void listObject();

	};
}

#endif