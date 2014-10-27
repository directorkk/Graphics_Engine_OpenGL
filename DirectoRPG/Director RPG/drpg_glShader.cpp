#include "drpg_glShader.h"
using namespace drpg;

drShader::drShader( const char* vname, const char* fname )
{
	std::string tmp; 

	vs = loadShader( tmp, GL_VERTEX_SHADER, vname );   // 編譯shader並且把id傳回vs 
	tmp = ""; 
	fs = loadShader( tmp, GL_FRAGMENT_SHADER, fname ); 

	program = glCreateProgram();   // 創建一個program 
	glAttachShader( program, vs );   // 把vertex shader跟program連結上 
	glAttachShader( program, fs );   // 把fragment shader跟program連結上 

	glLinkProgram( program );      // 根據被連結上的shader, link出各種processor 
}
drShader::~drShader()
{
	glDetachShader( program, vs ); 
	glDetachShader( program, fs ); 
	glDeleteShader( vs ); 
	glDeleteShader( fs ); 
	glDeleteProgram( program ); 
}

void drShader::loadFile( const char* filename, std::string &string )
{
	std::ifstream fp(filename); 
	if( !fp.is_open() ){ 
		std::cout << "Open <" << filename << "> error." << std::endl; 
		return; 
	} 

	char temp[300]; 
	while( !fp.eof() ){ 
		fp.getline( temp, 300 ); 
		string += temp; 
		string += '\n'; 
	} 

	fp.close(); 
}

unsigned int drShader::loadShader( std::string &source, int type, const char* filename )
{
	loadFile( filename, source );      // 把程式碼讀進source 

	unsigned int ShaderID; 
	ShaderID = glCreateShader( type );      // 告訴OpenGL我們要創的是哪種shader 

	const char* csource = source.c_str();   // 把std::string結構轉換成const char* 

	glShaderSource( ShaderID, 1, &csource, NULL );      // 把程式碼放進去剛剛創建的shader object中 
	glCompileShader( ShaderID );                  // 編譯shader 
	char error[1000] = ""; 
	glGetShaderInfoLog( ShaderID, 1000, NULL, error );   // 這是編譯過程的訊息, 錯誤什麼的把他丟到error裡面 
	std::cout << "File: <" << filename << "> Complie status: \n" << error << std::endl;   // 然後輸出出來 

	return ShaderID; 
}

void drShader::useShader()
{
	glUseProgram( program );
}

void drShader::delShader()
{
	glUseProgram( 0 ); 
}

unsigned int drShader::getProgramID()
{
	return program;
}