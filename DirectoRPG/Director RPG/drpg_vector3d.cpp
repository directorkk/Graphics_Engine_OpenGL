#include "drpg_vector3d.h"
using namespace drpg;

vector3::vector3()
{
	x = y = z = 0;
}

vector3::vector3( double initX )
{
	x = initX;
	y = z = 0;
}

vector3::vector3( double initX, double initY )
{
	x = initX;
	y = initY;
	z = 0;
}

vector3::vector3( double initX, double initY, double initZ )
{
	x = initX;
	y = initY;
	z = initZ;
}

vector3::~vector3()
{

}

void vector3::change( vector3 changeVec )
{
	x = changeVec.x;
	y = changeVec.y;
	z = changeVec.z;
}

void vector3::change( double changeX, double changeY, double changeZ  )
{
	x = changeX;
	y = changeY;
	z = changeZ;
}

void vector3::changeX( double changeX )
{
	x = changeX;
}

void vector3::changeY( double changeY )
{
	y = changeY;
}

void vector3::changeZ( double changeZ )
{
	z = changeZ;
}

double vector3::dot( double dotX, double dotY, double dotZ )
{
	return ( x*dotX + y*dotY + z*dotZ );
}

double vector3::dot( vector3 dotVec )
{
	return ( x*dotVec.x + y*dotVec.y + z*dotVec.z );
}

double vector3::length()
{
	return sqrt( x*x + y*y + z*z );
}

void vector3::normalize()
{
	double veclen = sqrt( x*x + y*y + z*z );
	if( veclen != 0 ){
		x *= 1/veclen;
		y *= 1/veclen;
		z *= 1/veclen;
	}
}

vector3 vector3::operator+( const vector3& anoVec )
{
	return vector3( x+anoVec.x, y+anoVec.y, z+anoVec.z );
}

vector3 vector3::operator-( const vector3& anoVec )
{
	return vector3( x-anoVec.x, y-anoVec.y, z-anoVec.z );
}

vector3 vector3::operator*( double num )
{
	return vector3( x*num, y*num, z*num );
}

vector3 vector3::operator/( double num )
{
	return vector3( x/num, y/num, z/num );
}

vector3& vector3::operator+=( const vector3& anoVec )
{
	x += anoVec.x;
	y += anoVec.y;
	z += anoVec.z;
	return *this;
}

vector3& vector3::operator-=( const vector3& anoVec )
{
	x -= anoVec.x;
	y -= anoVec.y;
	z -= anoVec.z;
	return *this;
}

vector3& vector3::operator*=( double num )
{
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

vector3& vector3::operator/=( double num )
{
	x /= num;
	y /= num;
	z /= num;
	return *this;
}	

bool vector3::operator==( const vector3 anoVec )
{
	return ( x == anoVec.x && y == anoVec.y && z == anoVec.z );
}

bool vector3::operator!=( const vector3 anoVec )
{
	return ( x != anoVec.x || y != anoVec.y || z != anoVec.z );
}

std::ostream& operator<<( std::ostream& out,const vector3& vec )
{
	out << vec.x << ", " << vec.y << ", " << vec.z <<std::endl;
	return out;
}