#ifndef _DRPG_OPENGL_MODEL_DATA_
#define _DRPG_OPENGL_MODEL_DATA_

#define MAX_GROUP 1500			// �̤j�q���X���
#define MAX_PLANE_OF_GROUP 196	// �̤j�q���ƶq�b�@�Ӷ��X��
#define MAX_VERTEX_OF_PLANE 40	// �̤j�q�I�ƶq�b�@�ӭ���
#define MAX_GROUP_SPHERE 50				// �̤j�q���X���, �y��
#define MAX_PLANE_OF_GROUP_SPHERE 1024	// �̤j�q���ƶq�b�@�Ӷ��X��, �y��
#define MAX_VERTEX_OF_PLANE_SPHERE 5		// �̤j�q�I�ƶq�b�@�ӭ���, �y��
#define MAX_MUTIGROUP 500		// �s��Group�̤j�ƶq
#define MAX_SORT_LIST 5000		// �禡Sort�i�ƦC���̤j�q�}�C
#define MAX_CHARACTER_OF_BUTTON_NAME 24	// ���s�W�٪��̤j�r��
#define MAX_CLASS_IN_SUBWIN 10			// �̤j�qClass�b��������
#define MAX_BUTTON_IN_SUBWIN 10			// �̤j�qButton�b��������
#define MAX_TEXTBOX_IN_SUBWIN 10		// �̤j�qTextbox�b��������
#define MAX_TEXTURE 1000		// �̤j�q�����x�s

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
		float TexCoord[MAX_VERTEX_OF_PLANE][2];		// ����ST�y��, �Ĥ@�}�C�PPoint�P�B, �ĤG�}�C [0]->s, [1]->t
		float TexCoordRate[2];						// ����j�p, [0]->s, [1]->t
		vector3 Nvector;							// Plane�����k�V�q, ������, [0]->x, [1]->y, [2]->z
		double Equation;

		Vertex Point[MAX_VERTEX_OF_PLANE];			// Plane�U�I, �̧ǥH�f�ɰw�ƦC
		int AMOUNT_POINT;
	};
	struct Group{	// Object���c
		bool Used;
		bool Selected;
		int Physics;

		vector3 Move;			// ���ʦV�q
		float Rotate;

		int MutiGroupNum;		// �@�P����s��
		char Name[15];			// Object�ۭq�W��
		vector3 O;				// Object����, [0]->x, [1]->y, [2]->z
		vector3 len;			// Object�d����e, [0]->x, [1]->y, [2]->z

		Plane Plane[MAX_PLANE_OF_GROUP];	// Object�U��
		int AMOUNT_PLANE;
	};


	struct Vertex_Sphere{
		bool Used;

		vector3 Site;		// �I�y��, [0]->x, [1]->y, [2]->z
		vector3 Nvector;							// Plane�����k�V�q, ������, [0]->x, [1]->y, [2]->z
	};

	struct Plane_Sphere{
		bool Used;

		int TexID;
		float TexCoord[MAX_VERTEX_OF_PLANE_SPHERE][2];		// ����ST�y��, �Ĥ@�}�C�PPoint�P�B, �ĤG�}�C [0]->s, [1]->t
		float TexCoordRate[2];						// ����j�p, [0]->s, [1]->t

		Vertex_Sphere Point[MAX_VERTEX_OF_PLANE_SPHERE];			// Plane�U�I, �̧ǥH�f�ɰw�ƦC
		int AMOUNT_POINT;
	};

	struct Group_Sphere{	// Object Sphere���c, �W�����Ƹ��h
		bool Used;
		bool Selected;

		int MutiGroupNum;		// �@�P����s��
		char Name[15];			// Object�ۭq�W��
		vector3 O;				// Object����, [0]->x, [1]->y, [2]->z
		vector3 len;			// Object�d����e, [0]->x, [1]->y, [2]->z

		Plane_Sphere Plane[MAX_PLANE_OF_GROUP_SPHERE];	// Object�U��
		int AMOUNT_PLANE;
	};
}

#endif