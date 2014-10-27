#ifndef _DRPG_OPENGL_SHADER_
#define _DRPG_OPENGL_SHADER_

#include "drpg_header.h"

namespace drpg
{
	class drShader
	{
		unsigned int vs, fs, program;
		void loadFile( const char* filename, std::string &string );
		unsigned int loadShader( std::string &source, int type, const char* filename ) ;
	public:
		drShader( const char* vname, const char* fname );
		~drShader();

		void useShader();
		void delShader();
		unsigned int getProgramID();
	};
}

#endif