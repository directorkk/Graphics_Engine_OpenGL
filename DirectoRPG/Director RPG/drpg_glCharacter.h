#ifndef _DRPG_OPENGL_CHARACTER_
#define _DRPG_OPENGL_CHARACTER_

#include "drpg_header.h"
#include "drpg_vector3d.h"
#include "drpg_glCamera.h"

namespace drpg
{
	class drCharacter : public drCamera
	{
		vector3 site;
		vector3 movement;
		
		double playerYaw, playerPitch;
		double camToplayerAngleXZ;
		vector3 camToplayerShift;
		bool isMove;
	public:
		drCharacter();
		drCharacter( vector3 locVec );
		drCharacter( vector3 locVec, double Yaw, double Pitch );
		drCharacter( vector3 locVec, vector3 shiftVec, double Yaw, double Pitch );
		~drCharacter();

		void ControlPlayer( bool* stateKeyboard );
		void UpdatePlayer();

		void Control( bool* stateKeyboard, int stateMouse, int windowSizeW = NULL, int windowSizeH = NULL, int bufferX = NULL, int bufferY = NULL );

		vector3 getSite();
		vector3 getMovement();
		double getPlayerYaw();
		double getPlayerPitch();

		void move( vector3 moveVec );
		void move( double moveX, double moveY, double moveZ );
		void setCtoPshift( vector3 shiftVec );
		void setCtoPshift( double shiftX, double shiftY, double shiftZ );
		void setSite( vector3 locVec );
		void setSite( double locX, double locY, double locZ );
		void pLookAt( double Yaw, double Pitch );
	};
}

#endif