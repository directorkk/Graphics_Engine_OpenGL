#ifndef _DRPG_OPENGL_MODEL_
#define _DRPG_OPENGL_MODEL_

#define DRPGM_MAX_GROUP 1500
#define DRPGM_MAX_GROUP_SPHERE 50

#include "drpg_header.h"
#include "drpg_glModelData.h"
#include "drpg_glTextureSource.h"
#include "drpg_glMesh.h"

namespace drpg
{
	struct planeData{
		unsigned int id, texID;
	};
	struct planeEqu{
		std::vector<vector3> coe;
		std::vector<vector3> result;
	};
	class drModel{

		std::vector<planeData> objList;
		std::vector<drMesh> object; 
		std::vector<planeEqu> objEqu;

		vector3 rVec, tVec, sVec;
		vector3 movement;

		glm::mat4 modelMatrix;

		int AMOUNT_GROUP;
		int AMOUNT_GROUP_SPHERE;
	public:
		drModel();
		drModel( const char* filename );
		~drModel();

		void draw( unsigned int programID, int textureMode, drTextureSource* textures, drTextureSource* normalTextures = NULL );
		void rotateX( double angle );
		void rotateY( double angle );
		void rotateZ( double angle );
		void translate( vector3 translateVec );
		void translate( double Tx, double Ty, double Tz );
		void scale( vector3 scaleVec );
		void scale( double Sx, double Sy, double Sz );
		// * 將vector加在現有的狀態上
		void updateState( vector3 translateVec, vector3 scaleVec, vector3 rotateVec );
		void updateNvector( Group* obj, Group_Sphere* objS );

		bool collisionDetect( vector3 subject );

		int getObjAmount();

		vector3 getTstate();
		vector3 getRstate();
		vector3 getSstate();

		void loadDrm( const char* filename );
		void Sort(float *list, int listcount, float &max, float &min);

	};
}

#endif