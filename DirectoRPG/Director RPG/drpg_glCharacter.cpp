#include "drpg_glCharacter.h"
using namespace drpg;


drCharacter::drCharacter()
{
	site.change( 0, 0, 0 );
	camToplayerShift.change( 0, 0, 0 );
	movement.change( 0, 0, 0 );
	playerPitch = playerYaw = camToplayerAngleXZ = 0;
}

drCharacter::drCharacter( vector3 locVec )
{
	site.change( locVec );
	camToplayerShift.change( 0, 0, 0 );
	movement.change( 0, 0, 0 );
	playerPitch = playerYaw = camToplayerAngleXZ = 0;
}

drCharacter::drCharacter( vector3 locVec, double Yaw, double Pitch )
{
	site.change( locVec );
	camToplayerShift.change( 0, 0, 0 );
	movement.change( 0, 0, 0 );
	playerPitch = Pitch;
	playerYaw = Yaw;
	camToplayerAngleXZ = 0;
}

drCharacter::drCharacter( vector3 locVec, vector3 shiftVec, double Yaw, double Pitch )
{
	site.change( locVec );
	camToplayerShift.change( shiftVec );
	movement.change( 0, 0, 0 );
	playerPitch = Pitch;
	playerYaw = Yaw;
	camToplayerAngleXZ = atan( camToplayerShift.x/camToplayerShift.z )/(PI/180);
}

drCharacter::~drCharacter()
{

}

void drCharacter::ControlPlayer( bool* stateKeyboard )
{

}

void drCharacter::UpdatePlayer()
{
	site += movement*getMoveRate();
}


void drCharacter::Control( bool* stateKeyboard, int stateMouse, int windowSizeW, int windowSizeH, int bufferX, int bufferY )
{
	if( stateMouse != CAM_NO_MOUSE_INPUT ){
	
	}

	if( stateKeyboard['w'] || stateKeyboard['W'] ){
	
	}
	if( stateKeyboard['s'] || stateKeyboard['S'] ){
	
	}
	if( stateKeyboard['a'] || stateKeyboard['A'] ){
	
	}
	if( stateKeyboard['d'] || stateKeyboard['D'] ){
	
	}

	glRotated( -getPitch(), 1, 0, 0 );
	glRotated( -getYaw(), 0, 1, 0 );
}

vector3 drCharacter::getSite()
{
	return site;
}

vector3 drCharacter::getMovement()
{
	return movement;
}

double drCharacter::getPlayerYaw()
{
	return playerYaw;
}

double drCharacter::getPlayerPitch()
{
	return playerPitch;
}


void drCharacter::move( vector3 moveVec )
{
	movement.change( moveVec );
}

void drCharacter::move( double moveX, double moveY, double moveZ )
{
	movement.change( moveX, moveY, moveZ );
}

void drCharacter::setCtoPshift( vector3 shiftVec )
{
	camToplayerShift.change( shiftVec );
	camToplayerAngleXZ = atan( camToplayerShift.x/camToplayerShift.z )/(PI/180);
}

void drCharacter::setCtoPshift( double shiftX, double shiftY, double shiftZ )
{
	camToplayerShift.change( shiftX, shiftY, shiftZ );
	camToplayerAngleXZ = atan( camToplayerShift.x/camToplayerShift.z )/(PI/180);
}

void drCharacter::setSite( vector3 locVec )
{
	site.change( locVec );
}

void drCharacter::setSite( double locX, double locY, double locZ )
{
	site.change( locX, locY, locZ );
}

void drCharacter::pLookAt( double Yaw, double Pitch )
{
	playerYaw = Yaw;
	playerPitch = Pitch;
}
