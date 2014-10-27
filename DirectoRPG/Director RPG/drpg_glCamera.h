#ifndef _DRPG_OPENGL_CAMERA_
#define _DRPG_OPENGL_CAMERA_

#include "drpg_header.h"
#include "drpg_vector3d.h"

namespace drpg
{
	enum CAM_InputType{
		CAM_NO_MOUSE_INPUT = 0,
		CAM_MOUSE_LEFT,
		CAM_MOUSE_RIGHT,
		CAM_WHEELMOUSE_UP,
		CAM_WHEELMOUSE_DOWN,
		CAM_MOUSE_MOTION_LEFT,
		CAM_MOUSE_MOTION_RIGHT
	};

	class drCamera
	{
		vector3 loc;
		vector3 backuploc;
		double camPitch, camYaw;
		double moveRate;
		double mouseRate;
		double howManyMove;

		void lockCamera();
		void moveCameraHori( double dir );
		void moveCameraVert( double dir );

		bool collision;
		bool lockY;

	public:
		drCamera();
		drCamera( vector3 loc);
		drCamera( vector3 loc, double yaw, double pitch );
		drCamera( vector3 loc, double yaw, double pitch, double moveR, double mouseR  );

		void Control( bool* stateKeyboard, int stateMouse, int windowSizeW = NULL, int windowSizeH = NULL, int bufferX = NULL, int bufferY = NULL );
		void UpdateCamera();
		void UpdateCamera( bool collisionDetect );

		vector3 getVector();
		vector3 getLocation();
		double getPitch();
		double getYaw();
		double getMoveRate();
		double getMouseRate();

		void setLocation(vector3 vec);
		void lookAt( double pitch, double yaw);
		void setSpeed( double moveR, double mouseR );
		void showLocation();
		void lockYxias( bool act );
	};
}
#endif
