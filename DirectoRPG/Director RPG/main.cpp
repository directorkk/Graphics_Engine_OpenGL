#include "drpgut.h"
using namespace drpg;

#define WINDOW_SIZE_W 1200
#define WINDOW_SIZE_H 700
#define WINDOW_VISION_ANGLE 45	// 攝影機視角
#define WINDOW_VISION_NEAR 0.5	// 攝影機最近視野
#define WINDOW_VISION_FAR 100001	// 攝影機最遠視野
#define SHADOW_MAP_RATE 4
#define FXAA_MAP_RATE 2

int windowWidth = WINDOW_SIZE_W,
	windowHeight = WINDOW_SIZE_H;
int shadowMapWidth = WINDOW_SIZE_W*SHADOW_MAP_RATE,
	shadowMapHeight = WINDOW_SIZE_H*SHADOW_MAP_RATE;
int fxaaWidth = WINDOW_SIZE_W*FXAA_MAP_RATE,
	fxaaHeight = WINDOW_SIZE_H*FXAA_MAP_RATE;
int panelWidth = 1900,
	panelHeight = 1000;

int AMOUNT_LIGHT = 1;
drEnity normalLight( DRE_LIGHT, vector3( -136.603, -16.24836, 90.0728 ) );

bool fxaa = false;
bool createMode = true;

drTextureSource* Texture;
drTextureSource* normalTexture;
drTextureSource* skybox;
drCamera* MainCamera;
drTextManager* textEngine;
drObjectPipeline* bgObjPipeline;
irrklang::ISoundEngine* bgmEngine = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();

vector3 lightPosition;

// * * * * * * FBO * * * * * * //
drFBO* mainFBO;
drFBO* fxaaFBO;
drFBO* shadowFBO;
drFBO* panelFBO;

// * * * * * * 著色器 * * * * * * //
drShader* skyboxShader;
drShader* mainShader;
drShader* normalMapShader;
drShader* quadRenderShader;
drShader* blurShader;
drShader* brightlightShader;
drShader* combineShader;
drShader* alphaShader;
drShader* fxaaShader;


// * * * * * * 模組 * * * * * * //
drModel box;
drModel wBox;
drModel bBox;
drModel castle;
drModel maze;


// * * * * * * 矩陣 * * * * * * //
glm::vec3 posiX( 1.0, 0.0, 0.0 );
glm::vec3 posiY( 0.0, 1.0, 0.0 );
glm::vec3 posiZ( 0.0, 0.0, 1.0 );
glm::mat4 projMatrix;
glm::mat4 modelViewMatrix;
glm::mat4 shadowMatrix;
GLdouble mvM[16], pM[16],
	objx, objy, objz;
GLint vp[4];

// * * * * * * 視窗控制 * * * * * * //
bool stateKeyboard[256] = {false};	// 按鍵狀態
bool mouseLeft = false;				// 左鍵狀態
bool mouseRight = false;			// 右鍵狀態
int mouseWheel = 0;					// 滾輪狀態
bool deviceInput = false;			// 裝置是否有輸入, 有則轉入處理函式
bool windowQuit = false;
bool unproj = false;

LRESULT CALLBACK windowProc( HWND, UINT, WPARAM, LPARAM );
void EnableOpenGL( HWND, HDC*, HGLRC* );	// 讓電腦知道你要使用OpenGL 
void DisableOpenGL( HWND, HDC , HGLRC );

bool InTheRange( double subject, double range1, double range2 );

clock_t timeBase, timeNow;
clock_t timeFrameS, timeFrameN;
clock_t timeKeyboardS, timeKeyboardN;
int controlFPS = 35;

int upx=0,upy=0;
float upz=0;

int mouse_x = 0, mouse_y = 0, old_mouse_x = 0, old_mouse_y = 0;
bool useMouse = true;

void Initialize()
{
	wchar_t buffer[128];
	GetModuleFileName( NULL, buffer, 128 );
	std::wstring tmpPath = buffer;
	unsigned int len = tmpPath.size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len+1];
	size_t charConvert = 0;
	wcstombs_s( &charConvert, p, len+1, tmpPath.c_str(), len ); 
    std::string actPath(p);
    delete[] p;
	for( unsigned int i=actPath.size()-1 ; ; i-- ){
		if( actPath[i] == '\\' )
			break;
		actPath.pop_back();
	}

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( WINDOW_VISION_ANGLE, (float)WINDOW_SIZE_W/(float)WINDOW_SIZE_H, WINDOW_VISION_NEAR, WINDOW_VISION_FAR );

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	std::string filename[6];
	bool isBmp = false;
	for( int i=0 ; i<6 ; i++ )
		filename[i] = "skybox\\sunny_002\\";
	filename[0] += "right";
	filename[1] += "left";
	filename[2] += "bottom";
	filename[3] += "top";
	filename[4] += "front";
	filename[5] += "back";
	for( int i=0 ; i<6 ; i++ ){
		std::string tmp = ".jpg";
		filename[i] += tmp;
		if( tmp == ".bmp" )
			isBmp = true;
	}
	normalLight.setLightColorL( vector3( 0.7, 0.7, 0.66 ), vector3( 0.8, 0.8, 0.8 ), vector3( 0.2, 0.2, 0.2 ) );
	normalLight.setLightColorM( vector3( 0.7, 0.7, 0.66 ), vector3( 0.5, 0.5, 0.5 ), vector3( 0.2, 0.2, 0.2 ) );
	normalLight.setLightDistance( 15 );

	Texture = new drTextureSource( "Texture\\texturelist.txt", "Texture\\" );
	normalTexture = new drTextureSource( "Texture\\texturelist.txt", "Texture\\Normal\\" );
	skybox = new drTextureSource( filename, isBmp );

	MainCamera = new drCamera( vector3( -234.223, -21.3852, 32.8659 ) );
	MainCamera->setSpeed( 0.5, 0.1 );

	textEngine = new drTextManager( L"華康秀風體W3" );
	textEngine->setTextHeight( 40 );
	textEngine->setTextWeight( 2 );

	bgObjPipeline = new drObjectPipeline();

	fxaaFBO = new drFBO( fxaaWidth, fxaaHeight );
	mainFBO = new drFBO( windowWidth, windowHeight );
	shadowFBO = new drFBO( shadowMapWidth, shadowMapHeight );

	glBindTexture( GL_TEXTURE_2D, shadowFBO->depthTex[0] );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glBindTexture( GL_TEXTURE_2D, 0 );	

	// sound
	bgmEngine->loadPlugins( "ikpMP3.dll" );
	{
		std::string tmp = actPath;
		tmp += "Sound\\N'to - Trauma (Worakls Remix).mp3";
		irrklang::ISoundSource* bgm = bgmEngine->addSoundSourceFromFile( tmp.c_str(), irrklang::ESM_AUTO_DETECT, true );
		bgmEngine->addSoundSourceAlias( bgm, "Trauma" );
		bgmEngine->setSoundVolume( 0.5f );
		bgmEngine->play2D( "Trauma", true );
	}
	soundEngine->loadPlugins( "ikpMP3.dll" );
	{
		std::string tmp = actPath;
		tmp += "Sound\\shake.mp3";
		irrklang::ISoundSource* sound = soundEngine->addSoundSourceFromFile( tmp.c_str(), irrklang::ESM_AUTO_DETECT, true );
		soundEngine->addSoundSourceAlias( sound, "shake" );
	}
	// shader
	skyboxShader = new drShader( "Shader\\skyboxShader.vs", "Shader\\skyboxShader.frag" );
	mainShader = new drShader( "Shader\\vertex.vs", "Shader\\fragment.frag" );
	normalMapShader = new drShader( "Shader\\normalMapShader.vs", "Shader\\normalMapShader.frag" );
	quadRenderShader = new drShader( "Shader\\quadRenderShader.vs", "Shader\\quadRenderShader.frag" );
	blurShader = new drShader( "Shader\\quadRenderShader.vs", "Shader\\blurShader.frag" );
	brightlightShader = new drShader( "Shader\\quadRenderShader.vs", "Shader\\brightlightShader.frag" );
	combineShader = new drShader( "Shader\\quadRenderShader.vs", "Shader\\combineShader.frag" );	
	alphaShader = new drShader( "Shader\\alphaShader.vs", "Shader\\alphaShader.frag" );
	fxaaShader = new drShader( "Shader\\fxaaShader.vs", "Shader\\fxaaShader.frag" );

	// model
	castle = drModel( "model\\castle006.drm" );
	maze = drModel( "model\\maze001.drm" );
	box = drModel( "model\\box.drm" );
	wBox = drModel( "model\\whitebox.drm" );
	bBox = drModel( "model\\blackbox.drm" );

	castle.scale( 0.05, 0.05, 0.05 );
	castle.translate( 0, 30, 0 );
	bgObjPipeline->addObject( castle, "castle1" );
	maze.scale( 0.05, 0.05, 0.05 );
	maze.translate( 0, 30, 0 );
	bgObjPipeline->addObject( maze, "maze1" );
	wBox.scale( 0.1, 0.1, 0.1 );
	wBox.translate( 61.5, 50, 208.5 );
	bgObjPipeline->addObject( wBox, "wBox" );
	bBox.scale( 0.1, 0.1, 0.1 );
	bBox.translate( -25.5, 50, 207.5 );
	bgObjPipeline->addObject( bBox, "bBox" );
	bBox.scale( 0.07, 0.07, 0.07 );
	bBox.translate( 1, 17, 94 );
	bgObjPipeline->addObject( bBox, "bBox0" );

	timeBase = clock();
	timeFrameS = clock();
	timeKeyboardS = clock();
}

void clean()
{
	delete Texture;
	delete normalTexture;
	delete skybox;
	delete MainCamera;
	delete textEngine;
	delete mainFBO;
	delete fxaaFBO;
	delete shadowFBO;
	delete bgmEngine;
	delete bgObjPipeline;
	delete soundEngine;
	// shader
	delete skyboxShader;
	delete mainShader;
	delete normalMapShader;
	delete quadRenderShader;
	delete blurShader;
	delete brightlightShader;
	delete combineShader;
	delete alphaShader;
	delete fxaaShader;
}

int x=0, y=0, z=0;

int frame = 0;
int FPS = 0;

float fly = 0;
bool updown = false;

void getShadowImage()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( WINDOW_VISION_ANGLE, (float)windowWidth/(float)windowHeight, WINDOW_VISION_NEAR, WINDOW_VISION_FAR );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_DEPTH_TEST );

	vector3 tmpVec( -124.867, 264.241, 299.119 ),
		tmpDir( -47.25, 327.75 );
	lightPosition.change( tmpVec );
	drCamera light;
	light.setLocation( tmpVec );
	light.lookAt( tmpDir.x, tmpDir.y );
	bool tmp[256] = { false };
	light.Control( tmp, CAM_NO_MOUSE_INPUT );
	light.UpdateCamera();

	glm::mat4 biasMatrix;
	biasMatrix[0][0]=0.5;biasMatrix[0][1]=0.0;biasMatrix[0][2]=0.0;biasMatrix[0][3]=0.0;
	biasMatrix[1][0]=0.0;biasMatrix[1][1]=0.5;biasMatrix[1][2]=0.0;biasMatrix[1][3]=0.0;
	biasMatrix[2][0]=0.0;biasMatrix[2][1]=0.0;biasMatrix[2][2]=0.5;biasMatrix[2][3]=0.0;
	biasMatrix[3][0]=0.5;biasMatrix[3][1]=0.5;biasMatrix[3][2]=0.5;biasMatrix[3][3]=1.0;

	glm::mat4 projM = glm::perspective( (float)WINDOW_VISION_ANGLE, (float)windowWidth/(float)windowHeight, (float)WINDOW_VISION_NEAR, (float)WINDOW_VISION_FAR );
	shadowMatrix = biasMatrix * projM * 
		glm::rotate( (float)-light.getPitch(), posiX ) * 
		glm::rotate( (float)-light.getYaw(), posiY ) *
		glm::translate( (float)-light.getLocation().x, (float)-light.getLocation().y, (float)-light.getLocation().z );


	shadowFBO->useFBO();
	glViewport( 0, 0, shadowMapWidth, shadowMapHeight );

	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	alphaShader->useShader();
	glUniform4f( glGetUniformLocation( alphaShader->getProgramID(), "color" ), 1.0, 0.0, 0.0, 1.0 );
	glClear( GL_DEPTH_BUFFER_BIT );

	bgObjPipeline->updateObject( DRO_ROTATE, "bBox0", 0, 1, 0, true );
	bgObjPipeline->updateObject( DRO_TRANSLATE, "bBox0", 1, 17+fly, 94 );
	bgObjPipeline->updateObject( DRO_ROTATE, "wBox", 0, 1, 0, true );
	bgObjPipeline->updateObject( DRO_ROTATE, "bBox", 0, -1, 0, true );
	bgObjPipeline->draw( alphaShader->getProgramID(), DRM_ONETEXTURE, Texture );

	alphaShader->delShader();
	glDisable( GL_CULL_FACE );
	glViewport( 0, 0, windowWidth, windowHeight );

	shadowFBO->delFBO();
}

void glRenderScene()
{
	normalLight.setLocation( vector3(
		MainCamera->getLocation().x + MainCamera->getVector().x*5,
		MainCamera->getLocation().y,
		MainCamera->getLocation().z + MainCamera->getVector().z*5
		) );

	frame++;
	if( updown )
		fly += 0.02;
	else
		fly -= 0.02;
	timeNow = clock();
	if( timeNow-timeBase >= 1000 ){
		timeBase = timeNow;
		FPS = frame;
		frame = 0;
	}
	if( fly >= 1 )
		updown = false;
	else if( fly <= -2 )
		updown = true;

	glEnable( GL_DEPTH_TEST );


	getShadowImage();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	glClearColor( 1.0, 1.0, 1.0, 1.0 ); 

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( WINDOW_VISION_ANGLE, (float)windowWidth/(float)windowHeight, WINDOW_VISION_NEAR, WINDOW_VISION_FAR );

	glMatrixMode( GL_MODELVIEW ); 
	glLoadIdentity(); 

	if( fxaa ){
		fxaaFBO->useFBO();
		glViewport( 0, 0, fxaaWidth, fxaaHeight );
	}
	else{
		mainFBO->useFBO();
		glViewport( 0, 0, windowWidth, windowHeight );
	}

	vector3 tmpVec( 2.44746, 16.685, 17.7786 ),
		tmpDir( -28.65, 25.15 );

	MainCamera->Control( stateKeyboard, CAM_NO_MOUSE_INPUT ); 

	// * * * * * * * * * * * * * * * * * * * * * * *
	// * ___SKYBOX___SHADER___                     *
	// * 
	skyboxShader->useShader(); 
	glDisable( GL_DEPTH_TEST );
	box.translate( 0, 0, 0 );
	box.rotateX( 180 );
	box.draw( skyboxShader->getProgramID(), DRM_CUBEMAPPING, skybox );
	glEnable( GL_DEPTH_TEST );
	skyboxShader->delShader();
	// *                                           *
	// * * * * * * * * * * * * * * * * * * * * * * *


	glClear( GL_DEPTH_BUFFER_BIT );
	MainCamera->UpdateCamera();


	// * * * * * * * * * * * * * * * * * * * * * * *
	// * ___GET__MATRICES___                       *
	// *
	{
		glm::vec3 locC( -MainCamera->getLocation().x, -MainCamera->getLocation().y, -MainCamera->getLocation().z );
		projMatrix = glm::perspective( (float)WINDOW_VISION_ANGLE, (float)windowWidth/(float)windowHeight, (float)WINDOW_VISION_NEAR, (float)WINDOW_VISION_FAR );
		modelViewMatrix = glm::rotate( (float)-MainCamera->getPitch(), posiX ) * glm::rotate( (float)-MainCamera->getYaw(), posiY ) * glm::translate( locC );
		glGetDoublev( GL_MODELVIEW_MATRIX, mvM );
		glGetDoublev( GL_PROJECTION_MATRIX, pM );
		glGetIntegerv( GL_VIEWPORT, vp );
	}
	// *                                           *
	// * * * * * * * * * * * * * * * * * * * * * * *	




	// * * * * * * * * * * * * * * * * * * * * * * *
	// * ___NORMALMAP___SHADER___                  *
	// * 
	normalMapShader->useShader();
	{
		// * * * * * * * * * * * * * * * * * *
		// | ___SHADER___UNIFORM___          |
		// |	
		glActiveTexture( GL_TEXTURE4 );
		glBindTexture( GL_TEXTURE_2D, shadowFBO->depthTex[0] );
		glUniform1i( glGetUniformLocation( normalMapShader->getProgramID(), "shadowMap" ), 4 );
		glUniform1i( glGetUniformLocation( normalMapShader->getProgramID(), "shadowMode" ), 1 );	
		glUniformMatrix4fv( glGetUniformLocation( normalMapShader->getProgramID(), "lightModelViewProjectionMatrix" ), 1, GL_FALSE,
			&shadowMatrix[0][0] );
		glUniformMatrix4fv( glGetUniformLocation( normalMapShader->getProgramID(), "lightMatrix" ), 1, GL_FALSE,
			&modelViewMatrix[0][0] );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "lightPos" ), (float)lightPosition.x, (float)lightPosition.y, (float)lightPosition.z );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "mambient" ), 0.4f, 0.4f, 0.37f );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "mdiffuse" ), 0.9f, 0.9f, 0.88f );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "mspecular" ), 1.0f, 1.0f, 1.0f );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "lambient" ), 0.5f, 0.5f, 0.5f );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "ldiffuse" ), 0.9f, 0.9f, 0.88f );
		glUniform3f( glGetUniformLocation( normalMapShader->getProgramID(), "lspecular" ), 0.5f, 0.5f, 0.4f );
		glUniform1f( glGetUniformLocation( normalMapShader->getProgramID(), "shininess" ), 32.0f );	

		if( normalLight.getState() ){
			glUniform1i( glGetUniformLocation( normalMapShader->getProgramID(), "AMOUNT_LIGHT" ), 1 );
			glm::vec4 tmpV4[100];
			tmpV4[0] = glm::vec4( normalLight.getLocation().x, normalLight.getLocation().y, normalLight.getLocation().z, 1.0 );
			tmpV4[0] = modelViewMatrix * tmpV4[0];
			glUniform4fv( glGetUniformLocation( normalMapShader->getProgramID(), "lightWorldSite" ), 100, &tmpV4[0][0] );
			glm::vec3 tmpV3[100];
			tmpV3[0] = glm::vec3(normalLight.getLightColorA().x, normalLight.getLightColorA().y, normalLight.getLightColorA().z );
			glUniform3fv( glGetUniformLocation( normalMapShader->getProgramID(), "nlambient" ), 100, &tmpV3[0][0] );
			tmpV3[0] = glm::vec3(normalLight.getLightColorD().x, normalLight.getLightColorD().y, normalLight.getLightColorD().z );
			glUniform3fv( glGetUniformLocation( normalMapShader->getProgramID(), "nldiffuse" ), 100, &tmpV3[0][0] );
			tmpV3[0] = glm::vec3(normalLight.getLightColorS().x, normalLight.getLightColorS().y, normalLight.getLightColorS().z );
			glUniform3fv( glGetUniformLocation( normalMapShader->getProgramID(), "nlspecular" ), 100, &tmpV3[0][0] );
			tmpV3[0] = glm::vec3(normalLight.getMaterialColorA().x, normalLight.getMaterialColorA().y, normalLight.getMaterialColorA().z );
			glUniform3fv( glGetUniformLocation( normalMapShader->getProgramID(), "nmambient" ), 100, &tmpV3[0][0] );
			tmpV3[0] = glm::vec3(normalLight.getMaterialColorD().x, normalLight.getMaterialColorD().y, normalLight.getMaterialColorD().z );
			glUniform3fv( glGetUniformLocation( normalMapShader->getProgramID(), "nmdiffuse" ), 100, &tmpV3[0][0] );
			tmpV3[0] = glm::vec3(normalLight.getMaterialColorS().x, normalLight.getMaterialColorS().y, normalLight.getMaterialColorS().z );
			glUniform3fv( glGetUniformLocation( normalMapShader->getProgramID(), "nmspecular" ), 100, &tmpV3[0][0] );
			glUniform1f( glGetUniformLocation( normalMapShader->getProgramID(), "MAX_DISTANCE" ), normalLight.getMaxDistance() );
		}
		else{
			glUniform1i( glGetUniformLocation( normalMapShader->getProgramID(), "AMOUNT_LIGHT" ), 0 );
		}

	}
	{
		// * * * * * * * * * * * * * * * * * *
		// | ___DRAW___SOMETHING___          |
		// |
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		bgObjPipeline->updateObject( DRO_ROTATE, "bBox0", 0, 1, 0, true );
		bgObjPipeline->updateObject( DRO_TRANSLATE, "bBox0", 1, 17+fly, 94 );
		bgObjPipeline->updateObject( DRO_ROTATE, "wBox", 0, 1, 0, true );
		bgObjPipeline->updateObject( DRO_ROTATE, "bBox", 0, -1, 0, true );
		bgObjPipeline->draw( normalMapShader->getProgramID(), DRM_NORMALMAPPING, Texture, normalTexture );
		glDisable( GL_CULL_FACE );
	}
	normalMapShader->delShader();
	// *                                           *
	// * * * * * * * * * * * * * * * * * * * * * * *


	// * * * * * * * * * * * * * * * * * * * * * * *
	// * ___QUAD__RENDER___SHADER___               *
	// * 
	quadRenderShader->useShader();
	{
		// * * * * * * * * * * * * * * * * * *
		// | ___DRAW___SOMETHING___          |
		// |

	}
	quadRenderShader->delShader();
	// *                                           *
	// * * * * * * * * * * * * * * * * * * * * * * *



	// 抓取滑鼠點擊位置, 在3D世界中的座標
	if( mouseLeft && !unproj ){
		unproj = true;
		glReadPixels( upx, WINDOW_SIZE_H-upy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &upz );
		gluUnProject( upx, WINDOW_SIZE_H-upy, upz, mvM, pM, vp, &objx, &objy, &objz );
	}


	if( fxaa )
		fxaaFBO->delFBO();
	else
		mainFBO->delFBO();

	glViewport( 0, 0, windowWidth, windowHeight );

	glDisable( GL_DEPTH_TEST );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( -1, 1, -1, 1 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();	

	if( fxaa ){
		fxaaShader->useShader();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, fxaaFBO->renderTex[0] );
		glUniform1i( glGetUniformLocation( fxaaShader->getProgramID(), "tex0" ), 0 );
		glUniform1f( glGetUniformLocation( fxaaShader->getProgramID(), "rt_w" ), (float)windowWidth );
		glUniform1f( glGetUniformLocation( fxaaShader->getProgramID(), "rt_h" ), (float)windowHeight );
	}
	else{
		quadRenderShader->useShader();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mainFBO->renderTex[0] );
		glUniform1i( glGetUniformLocation( quadRenderShader->getProgramID(), "tex" ), 0 );
		glUniform2f( glGetUniformLocation( quadRenderShader->getProgramID(), "pixelSize" ), 1.0f/(float)windowWidth, 1.0f/(float)windowHeight );
	}

	glBegin( GL_QUADS );
	glTexCoord2f( 0, 0 );
	glVertex3f( -1, -1, 0 );
	glTexCoord2f( 1, 0 );
	glVertex3f( 1, -1, 0 );
	glTexCoord2f( 1, 1 );
	glVertex3f( 1, 1, 0 );
	glTexCoord2f( 0, 1 );
	glVertex3f( -1, 1, 0 );	
	glEnd();

	if( fxaa )
		fxaaShader->delShader();
	else
		quadRenderShader->delShader();



	// * * * * * * * * * * * * * * * * * * * * * * *
	// * ___ALPHA___SHADER___                      *
	// * 
	alphaShader->useShader();
	{
		// * * * * * * * * * * * * * * * * * *
		// | ___DRAW___TEXT___               |
		// |
		glUniform4f( glGetUniformLocation( alphaShader->getProgramID(), "color" ), 0.0, 0.0, 0.0, 1.0 );
		textEngine->setTextPos( 10, WINDOW_SIZE_H-240+2 );
		textEngine->printf( "滑鼠<M>: %s\n創造模式<C>: %s\n手電筒<U>: %s\n抗鋸齒<空白鍵>: %s\nFPS: %d",
			useMouse ? "是" : "否",
			createMode ? "是" : "否",
			normalLight.getState() ? "是" : "否",
			fxaa ? "是" : "否",
			FPS );
		glUniform4f( glGetUniformLocation( alphaShader->getProgramID(), "color" ), 1.0, 1.0, 1.0, 1.0 );
		textEngine->setTextPos( 8, WINDOW_SIZE_H-240 );
		textEngine->printf( "滑鼠<M>: %s\n創造模式<C>: %s\n手電筒<U>: %s\n抗鋸齒<空白鍵>: %s\nFPS: %d",
			useMouse ? "是" : "否",
			createMode ? "是" : "否",
			normalLight.getState() ? "是" : "否",
			fxaa ? "是" : "否",
			FPS );
	}
	alphaShader->delShader();
	// *                                           *
	// * * * * * * * * * * * * * * * * * * * * * * *

}

void msgMouse( HWND hWnd, POINT pos )
{
	timeKeyboardN = clock();
	if( timeKeyboardN-timeKeyboardS >= 1000/60 ){
		timeKeyboardS = timeKeyboardN;
		mouse_x = pos.x - old_mouse_x; 
		mouse_y = pos.y - old_mouse_y; 

		if( useMouse ){
			if( mouseLeft ){
				MainCamera->Control( stateKeyboard, CAM_MOUSE_MOTION_LEFT, windowWidth, windowHeight, mouse_x, mouse_y ); 
			}
			if( mouseRight ){
				MainCamera->Control( stateKeyboard, CAM_MOUSE_MOTION_RIGHT, windowWidth, windowHeight, mouse_x, mouse_y );   
			}
			if( mouseWheel == 1 ){
				MainCamera->Control( stateKeyboard, CAM_WHEELMOUSE_UP ); 
			}
			if( mouseWheel == -1 ){
				MainCamera->Control( stateKeyboard, CAM_WHEELMOUSE_DOWN ); 
			}
		}
		else{
			mouse_x = WINDOW_SIZE_W/2 - pos.x; 
			mouse_y = WINDOW_SIZE_H/2 - pos.y; 
			MainCamera->Control( stateKeyboard, CAM_MOUSE_MOTION_LEFT, windowWidth, windowHeight, mouse_x, mouse_y );
			SetCursorPos(WINDOW_SIZE_W/2, WINDOW_SIZE_H/2);
		}

		old_mouse_x = pos.x; 
		old_mouse_y = pos.y; 
	}
}

void msgKeyboard( HWND hWnd, POINT pos )
{
	if( stateKeyboard[27] )		// ESC
		windowQuit = true;
	if( stateKeyboard['m'] ){
		useMouse = !useMouse;
		ShowCursor( useMouse );
	}
	if( stateKeyboard[' '] ){
		fxaa = !fxaa;
	}
	if( stateKeyboard['c'] ){
		createMode = !createMode;
		MainCamera->lockYxias( !createMode );
		MainCamera->setSpeed( createMode ? 0.5 : 0.3,
			createMode ? 0.1 : 0.05 );
	}
	if( stateKeyboard['8'] )
		std::cout << x << ", " << y << ", " << z << std::endl;
	if( stateKeyboard['u'] ){
		if( normalLight.getState() )
			normalLight.delEntity();
		else
			normalLight.useEntity();
	}
	if( stateKeyboard['r'] ){
		castle = drModel( "castle006.drm" );
		bgObjPipeline->delObject( "castle1" );
		castle.scale( 0.05, 0.05, 0.05 );
		castle.translate( 0, 30, 0 );
		bgObjPipeline->addObject( castle, "castle1" );
	}
}

int main( int argc, char** argv )
{
	HWND windowHandle;
	MSG windowMessage;
	WNDCLASS windowClass;

	HDC deviceHandle;
	HGLRC renderHandle;

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProc;
	windowClass.cbClsExtra = 0;	
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle( NULL );
	windowClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	windowClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"Director RPG";

	RegisterClass( &windowClass );

	windowHandle = CreateWindow(
		windowClass.lpszClassName,
		L"Director RPG",
		WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
		0, 0,
		WINDOW_SIZE_W, WINDOW_SIZE_H,
		NULL,
		NULL,
		windowClass.hInstance,
		NULL );

	//wglSwapIntervalEXT( 0 );
	EnableOpenGL( windowHandle, &deviceHandle, &renderHandle );
	ShowWindow( windowHandle, SW_SHOW );

	clock_t panelFrameS, panelFrameN;
	panelFrameS = clock();
	Initialize();
	while( !windowQuit ){
		if( PeekMessage( &windowMessage, NULL, 0, 0, PM_REMOVE ) ){
			if( windowMessage.message == WM_QUIT ){
				windowQuit = true;
			}
			else{
				DispatchMessage( &windowMessage );
			}
		}
		timeFrameN = clock();
		panelFrameN = clock();
		if( timeFrameN-timeFrameS >= 1000/controlFPS ){
			timeFrameS = timeFrameN;
			glRenderScene();
			SwapBuffers( deviceHandle ); 
		}
	}

	clean();
	DisableOpenGL( windowHandle, deviceHandle, renderHandle );
	DestroyWindow( windowHandle );
	UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );

	return 0;
}

LRESULT CALLBACK windowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	bool shiftPressed = GetKeyState( VK_SHIFT ) < 0;
	bool capsLockOn = GetKeyState( VK_CAPITAL ) > 0;

	if( msg == WM_CLOSE ){
		PostQuitMessage( 0 );
		return 0;
	}

	POINT pos;
	RECT rect;
	if( GetCursorPos( &pos ) && GetWindowRect( hWnd, &rect ) ){
		pos.x -= rect.left;
		pos.y -= rect.top;

		windowWidth = rect.right-rect.left;
		windowHeight = rect.bottom-rect.top;
	}

	// mouse input
	if( msg == WM_LBUTTONDOWN ||
		msg == WM_LBUTTONUP ||
		msg == WM_RBUTTONDOWN ||
		msg == WM_RBUTTONUP ||
		msg == WM_MOUSEWHEEL)
	{
		if( msg == WM_LBUTTONDOWN && !mouseLeft ){
			mouseLeft = true;
			old_mouse_x = pos.x; 
			old_mouse_y = pos.y;
			upx = pos.x;
			upy = pos.y;

		}
		if( msg == WM_LBUTTONUP && mouseLeft ){
			mouseLeft = false;
			mouse_x = 0; 
			mouse_y = 0; 
		}
		if( msg == WM_RBUTTONDOWN && !mouseRight ){
			mouseRight = true;
			old_mouse_x = pos.x; 
			old_mouse_y = pos.y;
		}
		if( msg == WM_RBUTTONUP && mouseRight ){
			mouseRight = false;
			mouse_x = 0; 
			mouse_y = 0; 
		}
		if( msg == WM_MOUSEWHEEL )
			if( wParam == 7864320 )
				mouseWheel = 1;
			else if( wParam == 4287102976 )
				mouseWheel = -1;
	}
	if( !useMouse ){
		old_mouse_x = pos.x; 
		old_mouse_y = pos.y;
	}
	msgMouse( hWnd, pos );
	mouseWheel = 0;

	// keyboard
	if( msg == WM_KEYDOWN ||
		msg == WM_KEYUP)
	{
		deviceInput = true;
		if( msg == WM_KEYDOWN ){
			//std::cout << msg << ", " << wParam << ", " << lParam << std::endl;
			if( InTheRange( wParam, 'A', 'Z' ) ){		// wParam只會出現大寫控碼, 所以要自行判斷大小寫
				if( shiftPressed ^ capsLockOn ){		// shift和caps假如只存在一個, 就是大寫
					stateKeyboard[wParam] = true;
					stateKeyboard[wParam+32] = true;
				}
				else{
					stateKeyboard[wParam] = true;
					stateKeyboard[wParam+32] = true;
				}
			}
			else{
				stateKeyboard[wParam] = true;
			}
		}
		if( msg == WM_KEYUP ){
			if( InTheRange( wParam, 'A', 'Z' ) ){
				if( shiftPressed ^ capsLockOn ){
					stateKeyboard[wParam] = false;		// 為防意外切換大小寫, 字母按鍵放開始同時關閉大小寫按鍵狀態
					stateKeyboard[wParam+32] = false;
				}
				else{
					stateKeyboard[wParam] = false;
					stateKeyboard[wParam+32] = false;
				}
			}
			else{
				stateKeyboard[wParam] = false;
			}
		}
	}
	if( deviceInput )
		msgKeyboard( hWnd, pos );
	deviceInput = false;

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void EnableOpenGL( HWND hWnd, HDC *hDC, HGLRC *hRC ) 
{ 
	PIXELFORMATDESCRIPTOR pfd; 
	int iFormat; 
	*hDC = GetDC( hWnd ); 

	// set the pixel format for the DC 
	ZeroMemory (&pfd, sizeof( pfd )); 
	pfd.nSize = sizeof( pfd ); 
	pfd.nVersion = 1; 
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
	pfd.iPixelType = PFD_TYPE_RGBA; 
	pfd.cColorBits = 24; 
	pfd.cDepthBits = 16; 
	pfd.iLayerType = PFD_MAIN_PLANE; 
	iFormat = ChoosePixelFormat ( *hDC, &pfd );   //由WINDOWS幫你找適合的pixel format 
	SetPixelFormat ( *hDC, iFormat, &pfd ); 

	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );         //wgl開頭的都是WINDOWS用來連接OpenGL用的 
	wglMakeCurrent( *hDC, *hRC ); 
} 

void DisableOpenGL( HWND hWnd, HDC hDC, HGLRC hRC ) 
{ 
	wglMakeCurrent( NULL, NULL ); 
	wglDeleteContext( hRC ); 
	ReleaseDC( hWnd, hDC ); 
}

bool InTheRange( double subject, double range1, double range2 )
{
	double max, min;

	if( range1 >= range2 ){
		max = range1;
		min = range2;
	}
	else{
		max = range2;
		min = range1;
	}

	if( subject <= max && subject >= min )
		return true;
	else
		return false;
}
