#ifndef _DRPG_VECTOR3D_H
#define _DRPG_VECTOR3D_H

#include <iostream>
#include <math.h>

namespace drpg
{
	class vector3
	{
	public:
		double x, y, z;
		vector3();
		vector3( double initX );
		vector3( double initX, double initY );
		vector3( double initX, double initY, double initZ );
		~vector3();

		void change( vector3 changeVec );
		void change( double changeX, double changeY, double changeZ );
		void changeX( double changeX );
		void changeY( double changeY );
		void changeZ( double changeZ );
		double dot( double dotX, double dotY, double dotZ );
		double dot( vector3 dotVec );
		double length();
		void normalize();

		vector3 operator+( const vector3& anoVec );
		vector3 operator-( const vector3& anoVec );
		vector3 operator*( double num );
		vector3 operator/( double num );

		vector3& operator+=( const vector3& anoVec );
		vector3& operator-=( const vector3& anoVec );
		vector3& operator*=( double num );
		vector3& operator/=( double num );	

		bool operator==( const vector3 anoVec );
		bool operator!=( const vector3 anoVec );

		friend std::ostream& operator<<(std::ostream& out,const vector3& vec);
	};
}

#endif