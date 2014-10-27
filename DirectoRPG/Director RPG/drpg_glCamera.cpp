#include "drpg_glCamera.h"
using namespace drpg;

drCamera::drCamera()
{
	loc.change( 0.0, 0.0, 0.0 );
	camPitch = 0;
	camYaw = 0;
	moveRate = 0.2;
	mouseRate = 0.1;
}
drCamera::drCamera( vector3 l )
{
	loc.change( l*-1 );
	camPitch = 0;
	camYaw = 0;
	moveRate = 0.2;
	mouseRate = 0.1;
}

drCamera::drCamera( vector3 l, double yaw, double pitch )
{
	loc.change( l*-1 );
	camPitch = pitch;
	camYaw = yaw;
	moveRate = 0.2;
	mouseRate = 0.1;
}

drCamera::drCamera( vector3 l, double yaw, double pitch, double moveR, double mouseR )
{
	loc.change( l*-1 );
	camPitch = pitch;
	camYaw = yaw;
	moveRate = moveR;
	mouseRate = mouseR;
}

void drCamera::lockCamera()
{
	if(camPitch > 90)
		camPitch = 90;
	if(camPitch < -90)
		camPitch = -90;
	if(camYaw < 0.0)
		camYaw += 360.0;
	if(camYaw > 360.0)
		camYaw -= 360;
}

void drCamera::moveCameraHori( double dir )
{
	if( !collision )
		backuploc.change( loc );
	double rad = (camYaw+dir)*PI/180.0;
	if( howManyMove != 0 ){
		loc.x += sin(rad)*moveRate/howManyMove;
		loc.z += cos(rad)*moveRate/howManyMove;
	}
}

void drCamera::moveCameraVert( double dir )
{
	if( !collision )
		backuploc.change( loc );
	double rad = (camPitch+dir)*PI/180.0;
	if( howManyMove != 0 && !lockY )
		loc.y -= sin(rad)*moveRate/howManyMove;	
}

double ptocRate = 0.7f;

void drCamera::Control( bool* stateKeyboard, int stateMouse, int windowSizeW, int windowSizeH, int bufferX, int bufferY )
{
	// mouse
	if( stateMouse != CAM_NO_MOUSE_INPUT )
	{
		if( stateMouse == CAM_MOUSE_MOTION_LEFT ){
			camYaw += mouseRate*(bufferX/2.0);
			camPitch += mouseRate*(bufferY/2.0);
		}
		if( stateMouse == CAM_WHEELMOUSE_UP ){	
			howManyMove++;	
			if( camPitch != 90 && camPitch != -90 )
				moveCameraHori(0.0);
			moveCameraVert(0.0);
		}
		if( stateMouse == CAM_WHEELMOUSE_DOWN ){	
			howManyMove++;
			if( camPitch != 90 && camPitch != -90 )
				moveCameraHori(180.0);
			moveCameraVert(180.0);
		}
		if( stateMouse == CAM_MOUSE_MOTION_RIGHT ){
			howManyMove++;
			if( bufferX > 0 )
				moveCameraHori(90.0);
			else if( bufferX < 0 )
				moveCameraHori(270.0);
		}

		lockCamera();
	}
	// keyboard
	if( stateKeyboard[(unsigned int)'w'] || stateKeyboard[(unsigned int)'W'] )
	{
		howManyMove++;
		if( camPitch != 90 && camPitch != -90 )
			moveCameraHori(0.0);
		moveCameraVert(0.0);
	}
	if( stateKeyboard[(unsigned int)'s'] || stateKeyboard[(unsigned int)'S'] )
	{
		howManyMove++;
		if( camPitch != 90 && camPitch != -90 )
			moveCameraHori(180.0);
		moveCameraVert(180.0);
	}
	if( stateKeyboard[(unsigned int)'a'] || stateKeyboard[(unsigned int)'A'] )
	{
		howManyMove++;
		moveCameraHori(90.0);
	}
	if( stateKeyboard[(unsigned int)'d'] || stateKeyboard[(unsigned int)'D'] )
	{
		howManyMove++;
		moveCameraHori(270.0);
	}
	if( stateKeyboard[(unsigned int)'h'] ||stateKeyboard[(unsigned int)'H']  )
	{
		showLocation();	
	}

	// result
	glRotatef( (float)-camPitch, 1.0f, 0.0f, 0.0f );
	glRotatef( (float)-camYaw, 0.0f, 1.0f, 0.0f );
}

void drCamera::UpdateCamera()
{	
	glTranslatef( (float)loc.x, (float)loc.y, (float)loc.z );
	howManyMove = 0;
}
void drCamera::UpdateCamera( bool collisionDetect )
{	
	if( !collisionDetect ){
		glTranslatef( (float)loc.x, (float)loc.y, (float)loc.z );
		collision = false;
	}
	else{
		glTranslatef( (float)backuploc.x, (float)backuploc.y, (float)backuploc.z );
		loc.change( backuploc );
		collision = true;
	}
	howManyMove = 0;
}

vector3 drCamera::getVector()
{
	vector3 tmp;
	tmp.x = cos(-camPitch*PI/180.0)*sin(-camYaw*PI/180.0);
	tmp.y = -sin(-camPitch*PI/180.0);
	tmp.z = -cos(-camPitch*PI/180.0)*cos(-camYaw*PI/180.0);

	return tmp;
}
vector3 drCamera::getLocation()
{
	return loc*-1;
}

double drCamera::getPitch()
{
	return camPitch;
}

double drCamera::getYaw()
{
	return camYaw;
}

double drCamera::getMoveRate()
{
	return moveRate;
}

double drCamera::getMouseRate()
{
	return mouseRate;
}

void drCamera::setLocation( vector3 vec )
{
	loc.change( vec*-1 );
}

void drCamera::lookAt( double pitch, double yaw )
{
	camPitch = pitch;
	camYaw = yaw;
}

void drCamera::setSpeed( double moveR, double mouseR )
{
	moveRate = moveR;
	mouseRate = mouseR;
}

void drCamera::showLocation()
{
	std::cout << "Location: " << -loc.x << ", " << -loc.y << ", " << -loc.z << std::endl
		<< "Direction: " << camPitch << ", " << camYaw << std::endl
		<< "Vector: " << getVector().x << ", " << getVector().y << ", " << getVector().z << std::endl;
}

void drCamera::lockYxias( bool act )
{
	lockY = act;
}