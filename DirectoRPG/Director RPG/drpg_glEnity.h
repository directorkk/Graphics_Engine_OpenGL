#ifndef _DRPG_OPENGL_ENITY_
#define _DRPG_OPENGL_ENITY_

#include "drpg_header.h"
#include "drpg_vector3d.h"

namespace drpg
{
	enum entityType{
		DRE_LIGHT = 0,
		DRE_LADDER,
		DRE_LOGIC
	};
	class drEnity
	{
		vector3 loc;
		int type;
		bool used;
		
		// * �O��
		vector3 lColorA, lColorD, lColorS,
			mColorA, mColorD, mColorS;
		float maxDistance;
		// * 
	public:
		drEnity( );
		drEnity( int entityType );
		drEnity( int entityType, vector3 location );
		~drEnity();
		
		void setLocation( vector3 locVec );
		vector3 getLocation();

		// * �]�m���u�C��, light color
		// * �̧����ҥ�, ���g��, �Ϯg��
		void setLightColorL( vector3 lamibient, vector3 ldiffuse, vector3 lspecular );
		// * �]�m���u�C��, material color
		// * �̧����ҥ�, ���g��, �Ϯg��
		void setLightColorM( vector3 mamibient, vector3 mdiffuse, vector3 mspecular );
		// * �]�m���u�i�ӶZ��
		void setLightDistance( float maxDist );
		vector3 getLightColorA();
		vector3 getLightColorD();
		vector3 getLightColorS();
		vector3 getMaterialColorA();
		vector3 getMaterialColorD();
		vector3 getMaterialColorS();

		float getMaxDistance();
		bool getState();

		void useEntity();
		void delEntity();
	};
}

#endif