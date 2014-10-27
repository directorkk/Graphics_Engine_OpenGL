#include "drpg_glTextureSource.h"
using namespace drpg;

drTextureSource::drTextureSource( const char* textureList, const char* sourcePath )
{
	amountTex = 0;
	if( sourcePath != NULL )
		loadTexture( textureList, sourcePath );
	else
		loadTexture( textureList );
}

drTextureSource::drTextureSource( std::string* cubemapList, bool isBmp )
{
	amountTex = 1;
	textureID = new unsigned int[amountTex];
	textureID[0] = loadCubemap( cubemapList, isBmp );
}

drTextureSource::drTextureSource( const char* sourcePath )
{
	amountTex = 1;
	textureID = new unsigned int[amountTex];

	float* priorities = new float[amountTex];
	priorities[0] = 1;

	glGenTextures( amountTex, textureID ); 
	glPrioritizeTextures( amountTex, textureID, priorities); 

	textureID[0] = ilutGLLoadImage( (wchar_t*)sourcePath );

	delete priorities;
}

drTextureSource::~drTextureSource()
{
	delete textureID;
}

void drTextureSource::loadTexture( const char* textureList, const char* sourcePath )
{
	std::fstream fp;
	fp.open( textureList, std::ios::in );	

	int i = 0;
	char line[128];	
	while( fp.getline(line, sizeof(line), '\n') ){
		amountTex++;
	}
	textureID = new unsigned int[amountTex];
	float* priorities = new float[amountTex];
	for( int j=0 ; j<amountTex ; j++ ){
		priorities[j] = 1;
	}

	glGenTextures( amountTex, textureID ); 
	glPrioritizeTextures( amountTex, textureID, priorities); 

	fp.clear();
	fp.seekg(0, std::ios::beg);
	while( fp.getline(line, sizeof(line), '\n') ){
		std::string tmp = "";
		if( sourcePath != NULL )
			tmp = sourcePath;
		tmp += line;
		textureID[i] = ilutGLLoadImage( (wchar_t*)tmp.c_str() );

		std::string tmpExt;
		for( int fnInx=tmp.length()-1 ; ; fnInx-- ){
			if( tmp[fnInx] == '.' ){
				for( int tmpInx=fnInx ; tmpInx<tmp.length() ; tmpInx++ )
					tmpExt += tmp[tmpInx];
				break;
			}
		}
		if( tmpExt != ".png" ){
			glBindTexture( GL_TEXTURE_2D, textureID[i] );
			//ilBindImage(textureID[i]);
			ILubyte* tmpData = ilGetData();
			int w = ilGetInteger( IL_IMAGE_WIDTH );
			int h = ilGetInteger( IL_IMAGE_HEIGHT );

			gluBuild2DMipmaps( GL_TEXTURE_2D,
						  GL_RGB,
						  w, h,
						  GL_RGB,
						  GL_UNSIGNED_BYTE,
						  tmpData );

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
		i++;
	}

	fp.close();

	delete priorities;
}

unsigned int drTextureSource::loadCubemap( std::string* filename, bool isBmp )
{
	unsigned int cubemapID;

	glGenTextures( 1, &cubemapID );
	glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapID );

	for( int i=0 ; i<6 ; i++ ){
		unsigned int tmpID; 
		int w = 0, h = 0;
		const char* tmpfilename = filename[i].c_str();
		tmpID = ilutGLLoadImage( (wchar_t*)tmpfilename );

		ILubyte* tmpData = ilGetData();
		w = ilGetInteger( IL_IMAGE_WIDTH );
		h = ilGetInteger( IL_IMAGE_WIDTH );
		if( isBmp )
			bitmapBGRtoRGB( tmpData, w, h );

		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, tmpData );

		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );		
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	return cubemapID;
}

void drTextureSource::bitmapBGRtoRGB( unsigned char* ucData, int w, int h ) 
{ 
	unsigned char change; 

	for( int i = 0, p = 0 ; i < w*h ; i++, p+=3 ) 
	{
		change = ucData[p]; 
		ucData[p] = ucData[p+2]; 
		ucData[p+2]  = change; 
	} 
} 