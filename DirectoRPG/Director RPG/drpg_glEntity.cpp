#include "drpg_glEnity.h"
using namespace drpg;


drEnity::drEnity( )
{
	loc.change( 0, 0, 0 );
	maxDistance = 100;
}
drEnity::drEnity( int entityType )
{
	type = entityType;
	loc.change( 0, 0, 0 );
	maxDistance = 100;
}
drEnity::drEnity( int entityType, vector3 location )
{
	type = entityType;
	loc.change( location );
	maxDistance = 100;
}
drEnity::~drEnity()
{

}

void drEnity::setLocation( vector3 locVec )
{
	loc.change( locVec );
}

void drEnity::setLightColorL( vector3 lamibient, vector3 ldiffuse, vector3 lspecular )
{
	lColorA.change( lamibient );
	lColorD.change( ldiffuse );
	lColorS.change( lspecular );
}
void drEnity::setLightColorM( vector3 mamibient, vector3 mdiffuse, vector3 mspecular )
{
	mColorA.change( mamibient );
	mColorD.change( mdiffuse );
	mColorS.change( mspecular );
}
void drEnity::setLightDistance( float maxDist )
{
	maxDistance = maxDist;
}

bool drEnity::getState()
{
	return used;
}

void drEnity::useEntity()
{
	used = true;
}

void drEnity::delEntity()
{
	used = false;
}

vector3 drEnity::getLocation()
{
	return loc;
}
vector3 drEnity::getLightColorA()
{
	return lColorA;
}
vector3 drEnity::getLightColorD()
{
	return lColorD;
}
vector3 drEnity::getLightColorS()
{
	return lColorS;
}
vector3 drEnity::getMaterialColorA()
{
	return mColorA;
}
vector3 drEnity::getMaterialColorD()
{
	return mColorD;
}
vector3 drEnity::getMaterialColorS()
{
	return mColorS;
}
float drEnity::getMaxDistance()
{
	return maxDistance;
}