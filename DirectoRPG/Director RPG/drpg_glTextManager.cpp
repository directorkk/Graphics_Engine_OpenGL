#include "drpg_glTextManager.h"
using namespace drpg;


drTextManager::drTextManager()
{
	fontHeight = 16;
	fontWeight = 0;
	fontItalic = FALSE;
	fontUnderline = FALSE;
	fontStrikeOut = FALSE;

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	fontType.push_back( L"Courier New" );
	LPCWSTR tmp = fontType[fontType.size()-1].c_str();
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );
	fontUsed = 0;
	setPos = true;
}

drTextManager::drTextManager( wchar_t* fontName )
{
	fontHeight = 16;
	fontWeight = 0;
	fontItalic = FALSE;
	fontUnderline = FALSE;
	fontStrikeOut = FALSE;

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	fontType.push_back( fontName );
	LPCWSTR tmp = fontType[fontType.size()-1].c_str();
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );
	fontUsed = 0;
	setPos = true;
}

drTextManager::drTextManager( std::wstring fontName )
{
	fontHeight = 16;
	fontWeight = 0;
	fontItalic = FALSE;
	fontUnderline = FALSE;
	fontStrikeOut = FALSE;

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	fontType.push_back( fontName );
	LPCWSTR tmp = fontType[fontType.size()-1].c_str();
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );
	fontUsed = 0;
	setPos = true;
}

drTextManager::~drTextManager()
{

}

int drTextManager::compare( wchar_t* fontName )
{
	for( unsigned int i=0 ; i<fontType.size() ; i++ ){
		if( fontName == fontType[i] )
			return i;
	}
	return -1;
}
int drTextManager::compare( std::wstring fontName )
{
	for( unsigned int i=0 ; i<fontType.size() ; i++ ){
		if( fontName == fontType[i] )
			return i;
	}
	return -1;
}

bool drTextManager::bindFontType( int fontID )
{
	if( fontID >= (int)fontType.size() )
		return false;

	glDeleteLists( font, 65536 ); 
	font = 0;
	memset( loaded, 0, 65536*sizeof(bool) );
	DeleteObject( hFont );

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	LPCWSTR tmp = fontType[fontID].c_str();
	fontHeight = 16;
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );
	fontUsed = fontID;

	return true;
}
bool drTextManager::bindFontType( wchar_t* fontName )
{
	LPCWSTR tmp = NULL;
	if( compare( fontName ) == -1 ){
		fontUsed = addFontType( fontName );
		tmp = fontType[fontUsed].c_str();
	}
	else{
		if( fontUsed == compare( fontName ) )
			return false;
		tmp = fontType[compare( fontName )].c_str();
	}

	glDeleteLists( font, 65536 ); 
	font = 0;
	memset( loaded, 0, 65536*sizeof(bool) );
	DeleteObject( hFont );

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	fontHeight = 16;
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );

	return true;
}
bool drTextManager::bindFontType( std::wstring fontName )
{
	LPCWSTR tmp = NULL;
	if( compare( fontName ) == -1 ){
		fontUsed = addFontType( fontName );
		tmp = fontType[fontUsed].c_str();
	}
	else{
		if( fontUsed == compare( fontName ) )
			return false;
		tmp = fontType[compare( fontName )].c_str();
	}

	glDeleteLists( font, 65536 ); 
	font = 0;
	memset( loaded, 0, 65536*sizeof(bool) );
	DeleteObject( hFont );

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	fontHeight = 16;
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );

	return true;
}

bool drTextManager::setTextPos( vector3 printPos )
{
	loc.change( printPos );
	loc.changeZ( 0 );
	setPos = true;

	return true;
}
bool drTextManager::setTextPos( double printX, double printY )
{
	loc.change( printX, printY, 0 );
	setPos = true;

	return true;
}

bool drTextManager::setTextHeight( int fontH )
{
	if( fontH == fontHeight )
		return false;
	glDeleteLists( font, 65536 ); 
	font = 0;
	memset( loaded, 0, 65536*sizeof(bool) );
	DeleteObject( hFont );

	DWORD fdwCharSet = CHINESEBIG5_CHARSET;
	LPCWSTR tmp = fontType[fontUsed].c_str();
	fontHeight = fontH;
	font = glGenLists( 65536 );
	hFont = CreateFont( 
		fontHeight,
		0,
		0,
		0, 
		fontWeight,
		fontItalic,
		fontUnderline,
		fontStrikeOut, 
		fdwCharSet, 
		OUT_TT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH, 
		tmp );
	fontUsed = fontType.size()-1;

	return true;
}

bool drTextManager::setTextWeight( int fontW )
{
	if( fontW > 9 || fontW < 0 )
		return false;

	fontWeight = fontW*100;

	return true;
}

bool drTextManager::setTextModel( bool italic, bool underline, bool strikeout )
{
	fontItalic = italic;
	fontUnderline = underline;
	fontStrikeOut = strikeout;

	return true;
}

bool drTextManager::printf( const char* str, ... )
{
setPosRedisplay:
	char buffer[65536];
	memset( loaded, 0, 65536*sizeof(bool) );

	va_list args;
	va_start(args, str);
	vsprintf_s( buffer, str, args );
	va_end(args);

	float p0[4], p1[4], c0[4] = { 1, 1, 1, 1 };
	int i = 0, j = 0, len = 0;
	wchar_t *wstr;
	GLint viewport[4];
	HDC hdc = 0;

	glGetIntegerv( GL_VIEWPORT, viewport );
	glGetFloatv( GL_CURRENT_RASTER_POSITION, p0 );

	glPushAttrib( GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, viewport[2], 0, viewport[3] ); 

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if( setPos )
	{
		glRasterPos2d( loc.x, viewport[3]-loc.y-fontHeight );
	}
	else
	{
		glRasterPos2f( p0[0], p0[1] );
		glGetFloatv( GL_CURRENT_RASTER_COLOR, c0 );
		len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, 0, 0);
		wstr = (wchar_t*)malloc( len*sizeof(wchar_t) );
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wstr, len);
		glColor4fv( c0 );
		glRasterPos2f( p0[0], p0[1] );
		for( i=0; i<len-1; i++ )
		{
			if( wstr[i]=='\n' )
			{
				glGetFloatv( GL_CURRENT_RASTER_POSITION, (float*)&p1 );
				glRasterPos2d( loc.x, p1[1]-(fontHeight+2) );
			}else
			{
				if( !loaded[wstr[i]] )
				{
					if( hdc==0 )
					{
						hdc = wglGetCurrentDC();
						SelectObject( hdc, hFont );
					}
					wglUseFontBitmapsW( hdc, wstr[i], 1, font+wstr[i] );
					loaded[wstr[i]] = true;
				}
				glCallList(font+wstr[i]);
			}
		}
		free(wstr);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

	if( setPos ){
		setPos = false;
		goto setPosRedisplay;
	}


	return true;
}
bool drTextManager::printf( std::string str, ... )
	// °Ñ¦Ò¦Ûma_hty, http://www.programmer-club.com/ShowSameTitleN/opengl/2650.html
{
setPosRedisplay:
	char buffer[65536];
	memset( loaded, 0, 65536*sizeof(bool) );

	va_list args;
	va_start( args, str );
	vsprintf_s( buffer, str.c_str(), args );
	va_end(args);

	float p0[4], p1[4], c0[4] = { 1, 1, 1, 1 };
	int i = 0, j = 0, len = 0;
	wchar_t *wstr;
	GLint viewport[4];
	HDC hdc = 0;

	glGetIntegerv( GL_VIEWPORT, viewport );
	glGetFloatv( GL_CURRENT_RASTER_POSITION, p0 );

	glPushAttrib( GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D( 0, viewport[2], 0, viewport[3] ); 

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if( setPos )
	{
		glRasterPos2d( loc.x, viewport[3]-loc.y-fontHeight );
	}
	else
	{
		glRasterPos2f( p0[0], p0[1] );
		glGetFloatv( GL_CURRENT_RASTER_COLOR, c0 );
		len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, 0, 0);
		wstr = (wchar_t*)malloc( len*sizeof(wchar_t) );
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, buffer, -1, wstr, len);
		glColor4fv( c0 );
		glRasterPos2f( p0[0], p0[1] );
		for( i=0; i<len-1; i++ )
		{
			if( wstr[i]=='\n' )
			{
				glGetFloatv( GL_CURRENT_RASTER_POSITION, (float*)&p1 );
				glRasterPos2d( loc.x, p1[1]-(fontHeight+2) );
			}else
			{
				if( !loaded[wstr[i]] )
				{
					if( hdc==0 )
					{
						hdc = wglGetCurrentDC();
						SelectObject( hdc, hFont );
					}
					wglUseFontBitmapsW( hdc, wstr[i], 1, font+wstr[i] );
					loaded[wstr[i]] = true;
				}
				glCallList(font+wstr[i]);
			}
		}
		free(wstr);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

	if( setPos ){
		setPos = false;
		goto setPosRedisplay;
	}


	return true;
}

int drTextManager::addFontType( wchar_t* fontName )
{
	if( compare( fontName ) == -1 ){
		fontType.push_back( fontName );
		return fontType.size()-1;
	}
	return compare( fontName );
}
int drTextManager::addFontType( std::wstring fontName )
{
	if( compare( fontName ) == -1 ){
		fontType.push_back( fontName );
		return fontType.size()-1;
	}
	return compare( fontName );
}

void drTextManager::getListFontType( std::string* fontList, unsigned int sizeList )
{
	int tmpListAmount = 0;
	if( sizeList < fontType.size() )
		tmpListAmount = sizeList;
	else
		tmpListAmount = fontType.size();
	for( int i=0 ; i<tmpListAmount ; i++ ){
		std::string tmp = "";
		for( unsigned int j=0 ; j<fontType[i].size() ; j++ )
			tmp += (char)fontType[i][j];
		fontList[i] = tmp;
		tmp = "";
	}
}

int drTextManager::getAmountFont()
{
	return fontType.size();
}

std::string drTextManager::getFontType()
{
	unsigned int len = fontType[fontUsed].size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len+1];
	size_t charConvert = 0;
	wcstombs_s( &charConvert, p, len+1, fontType[fontUsed].c_str(), len ); 
    std::string tmp(p);
    delete[] p;

	return tmp;
}

int drTextManager::getFontHeight()
{
	return fontHeight;
}

int drTextManager::getFontWeight()
{
	return fontWeight/100;
}

bool drTextManager::getStateItalic()
{
	return fontItalic;
}

bool drTextManager::getStateUnderline()
{
	return fontUnderline;
}

bool drTextManager::getStateStrikeOut()
{
	return fontStrikeOut;
}
