#ifndef _DRPG_TEXTURE_SOURCE_
#define _DRPG_TEXTURE_SOURCE_

#include "drpg_header.h"

namespace drpg
{
	class drTextureSource
	{
		int amountTex;
		void loadTexture( const char* textureList, const char* sourcePath = NULL );
		unsigned int loadCubemap( std::string* filename, bool isBmp );
		void bitmapBGRtoRGB( unsigned char* ucData, int w, int h );
	public:
		unsigned int* textureID;
		unsigned int* shininess;
		// * �q����ascii�s�X������qŪ������
		// * ��󤺮e���u�]�t���ɸ��|
		// * ->�d��, ��Ū���P�ؿ�Std��Ƨ�����Std_NoFile.bmp
		// * ->�h��󤺫h�ݼФJ: Std\\Std_NoFile.bmp
		// * �ޤJ��i�Q��textureID���o���誺�W�l
		// * �t�i������drpg_ModelSource������ø�Ϯɨϥ�
		// * �Y�x�s���誺��Ƨ��Q���������Ϊ��l��Ƨ����x�s����������Ƨ�
		// * �i�Q��sourcePath�ǤJ����Ƨ��W��, �Ӥ��Φb��󤤥����K�W
		// * ->�d��, ����Ƨ��W�٬�Texture
		// * ->�hsourcePath�i�q��: "Texture\\"
		// * �YŪ�Jnormalmap texture
		// * �аw��C�i����̧��B�~�K�W�ϥ���1~64(�Ʀr�U�p�U�G)
		// * ���ɮ�Shiniess���(�Y�S���Цۦ�Ы�)
		drTextureSource( const char* textureList, const char* sourcePath = NULL );

		// * ���J�@��cubemap
		// * �ǤJ�̧Ǭ�right, left, bottom, top, front, back�����|�}�C
		// * �Y���ɦW��bmp�ݼаOtrue
		drTextureSource( std::string* cubemapList, bool isBmp = false );

		// * ��J�@�Ӹ��|Ū���@�i2D����
		drTextureSource( const char* sourcePath );
		~drTextureSource();
	};
}

#endif