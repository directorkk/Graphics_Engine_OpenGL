#include "drpg_glObjectPipeline.h"
using namespace drpg;


drObjectPipeline::drObjectPipeline()
{

}

drObjectPipeline::~drObjectPipeline()
{

}


void drObjectPipeline::draw( unsigned int programID, int textureMode, drTextureSource* textures, drTextureSource* normalTextures )
{
	for( unsigned int i=0 ; i<objectlist.size() ; i++ ){
		objectlist[i].draw( programID, textureMode, textures, normalTextures );
	}
}


bool drObjectPipeline::addObject( drModel obj, const char* objName )
{
	if( std::find( namelist.begin(), namelist.end(), std::string( objName ) ) == namelist.end() ){
		objectlist.push_back( obj );
		namelist.push_back( objName );
		return true;
	}
	return false;
}

bool drObjectPipeline::delObject( const char* objName )
{
	unsigned int index = std::find( namelist.begin(), namelist.end(), std::string( objName ) )-namelist.end()+namelist.size();
	if( index < namelist.size() ){
		objectlist.erase( objectlist.begin()+index );
		namelist.erase( namelist.begin()+index );
		return true;
	}
	return false;
}

bool drObjectPipeline::updateObject( drModel obj, const char* objName )
{
	unsigned int index = std::find( namelist.begin(), namelist.end(), std::string( objName ) )-namelist.end()+namelist.size();
	if( index < namelist.size() ){
		objectlist.erase( objectlist.begin()+index );
		objectlist.insert( objectlist.begin()+index, obj );
		return true;
	}
	return false;
}
bool drObjectPipeline::updateObject( int updateType, const char* objName, double updateX, double updateY, double updateZ, bool addToNow )
{
	unsigned int index = std::find( namelist.begin(), namelist.end(), std::string( objName ) )-namelist.end()+namelist.size();
	if( index < namelist.size() ){
		if( updateType == DRO_ROTATE ){
			if( addToNow ){
				objectlist[index].updateState( vector3( 0, 0, 0 ), vector3( 0, 0, 0 ), vector3( updateX, updateY, updateZ ) );
			}
			else{
				objectlist[index].rotateX( updateX );
				objectlist[index].rotateY( updateY );
				objectlist[index].rotateZ( updateZ );
			}
		}
		if( updateType == DRO_TRANSLATE ){
			if( addToNow ){
				objectlist[index].updateState( vector3( updateX, updateY, updateZ ), vector3( 0, 0, 0 ), vector3( 0, 0, 0 ) );
			}
			else{
				objectlist[index].translate( updateX, updateY, updateZ );
			}
		}
		if( updateType == DRO_SCALE ){
			if( addToNow ){
				objectlist[index].updateState( vector3( 0, 0, 0 ), vector3( updateX, updateY, updateZ ), vector3( 0, 0, 0 ) );
			}
			else{
				objectlist[index].scale( updateX, updateY, updateZ );
			}
		}
		return true;
	}
	return false;
}
bool drObjectPipeline::updateObject( int updateType, const char* objName, vector3 updateVec, bool addToNow )
{
	unsigned int index = std::find( namelist.begin(), namelist.end(), std::string( objName ) )-namelist.end()+namelist.size();
	if( index < namelist.size() ){
		if( updateType == DRO_ROTATE ){
			if( addToNow ){
				objectlist[index].updateState( vector3( 0, 0, 0 ), vector3( 0, 0, 0 ), updateVec );
			}
			else{
				objectlist[index].rotateX( updateVec.x );
				objectlist[index].rotateY( updateVec.y );
				objectlist[index].rotateZ( updateVec.z );
			}
		}
		if( updateType == DRO_TRANSLATE ){
			if( addToNow ){
				objectlist[index].updateState( updateVec, vector3( 0, 0, 0 ), vector3( 0, 0, 0 ) );
			}
			else{
				objectlist[index].translate( updateVec );
			}
		}
		if( updateType == DRO_SCALE ){
			if( addToNow ){
				objectlist[index].updateState( vector3( 0, 0, 0 ), updateVec, vector3( 0, 0, 0 ) );
			}
			else{
				objectlist[index].scale( updateVec );
			}
		}
		return true;
	}
	return false;
}
void drObjectPipeline::listObject()
{
	for( unsigned int i=0 ; i<namelist.size() ; i++ ){
		std::cout << namelist[i] << std::endl;
	}
}

vector3 drObjectPipeline::getState( int stateType, const char* objName )
{
	unsigned int index = std::find( namelist.begin(), namelist.end(), std::string( objName ) )-namelist.end()+namelist.size();
	if( index < namelist.size() ){
		if( stateType == DRO_ROTATE ){
			return objectlist[index].getRstate();
		}
		if( stateType == DRO_TRANSLATE ){
			return objectlist[index].getTstate();
		}
		if( stateType == DRO_SCALE ){
			return objectlist[index].getSstate();
		}
	}
	return vector3( 0, 0, 0 );
}