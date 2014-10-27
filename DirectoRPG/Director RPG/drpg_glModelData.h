#ifndef _DRPG_OPENGL_MODEL_DATA_
#define _DRPG_OPENGL_MODEL_DATA_

#define MAX_GROUP 1500			// 最大量集合方塊
#define MAX_PLANE_OF_GROUP 196	// 最大量面數量在一個集合中
#define MAX_VERTEX_OF_PLANE 40	// 最大量點數量在一個面中
#define MAX_GROUP_SPHERE 50				// 最大量集合方塊, 球形
#define MAX_PLANE_OF_GROUP_SPHERE 1024	// 最大量面數量在一個集合中, 球形
#define MAX_VERTEX_OF_PLANE_SPHERE 5		// 最大量點數量在一個面中, 球形
#define MAX_MUTIGROUP 500		// 群組Group最大數量
#define MAX_SORT_LIST 5000		// 函式Sort可排列的最大量陣列
#define MAX_CHARACTER_OF_BUTTON_NAME 24	// 按鈕名稱的最大字元
#define MAX_CLASS_IN_SUBWIN 10			// 最大量Class在內視窗中
#define MAX_BUTTON_IN_SUBWIN 10			// 最大量Button在內視窗中
#define MAX_TEXTBOX_IN_SUBWIN 10		// 最大量Textbox在內視窗中
#define MAX_TEXTURE 1000		// 最大量材質儲存

#include "drpg_header.h"

namespace drpg
{
	struct vertexData{
		vector3 position;
		vector3 normal;
		vector3 tangent;
		vector3 color;
		float U, V;
	};

	struct textureData{
		unsigned int id;
		unsigned int type;
	};

	struct Vertex{
		bool Used;

		vector3 Site;
	};
	struct Plane{
		bool Used;

		int TexID;
		float TexCoord[MAX_VERTEX_OF_PLANE][2];		// 材質ST座標, 第一陣列與Point同步, 第二陣列 [0]->s, [1]->t
		float TexCoordRate[2];						// 材質大小, [0]->s, [1]->t
		vector3 Nvector;							// Plane正面法向量, 打光用, [0]->x, [1]->y, [2]->z
		double Equation;

		Vertex Point[MAX_VERTEX_OF_PLANE];			// Plane各點, 依序以逆時針排列
		int AMOUNT_POINT;
	};
	struct Group{	// Object結構
		bool Used;
		bool Selected;
		int Physics;

		vector3 Move;			// 移動向量
		float Rotate;

		int MutiGroupNum;		// 共同選取群組
		char Name[15];			// Object自訂名稱
		vector3 O;				// Object中心, [0]->x, [1]->y, [2]->z
		vector3 len;			// Object範圍長寬, [0]->x, [1]->y, [2]->z

		Plane Plane[MAX_PLANE_OF_GROUP];	// Object各面
		int AMOUNT_PLANE;
	};


	struct Vertex_Sphere{
		bool Used;

		vector3 Site;		// 點座標, [0]->x, [1]->y, [2]->z
		vector3 Nvector;							// Plane正面法向量, 打光用, [0]->x, [1]->y, [2]->z
	};

	struct Plane_Sphere{
		bool Used;

		int TexID;
		float TexCoord[MAX_VERTEX_OF_PLANE_SPHERE][2];		// 材質ST座標, 第一陣列與Point同步, 第二陣列 [0]->s, [1]->t
		float TexCoordRate[2];						// 材質大小, [0]->s, [1]->t

		Vertex_Sphere Point[MAX_VERTEX_OF_PLANE_SPHERE];			// Plane各點, 依序以逆時針排列
		int AMOUNT_POINT;
	};

	struct Group_Sphere{	// Object Sphere結構, 上限面數較多
		bool Used;
		bool Selected;

		int MutiGroupNum;		// 共同選取群組
		char Name[15];			// Object自訂名稱
		vector3 O;				// Object中心, [0]->x, [1]->y, [2]->z
		vector3 len;			// Object範圍長寬, [0]->x, [1]->y, [2]->z

		Plane_Sphere Plane[MAX_PLANE_OF_GROUP_SPHERE];	// Object各面
		int AMOUNT_PLANE;
	};
}

#endif