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
		// * 從任何ascii編碼的文件批量讀取材質
		// * 文件內容須只包含圖檔路徑
		// * ->範例, 欲讀取同目錄Std資料夾中的Std_NoFile.bmp
		// * ->則文件內則需標入: Std\\Std_NoFile.bmp
		// * 引入後可利用textureID取得材質的名子
		// * 另可直接於drpg_ModelSource的物件繪圖時使用
		// * 若儲存材質的資料夾被分做分類用的子資料夾及儲存全部的母資料夾
		// * 可利用sourcePath傳入母資料夾名稱, 而不用在文件中全部添上
		// * ->範例, 母資料夾名稱為Texture
		// * ->則sourcePath可訂為: "Texture\\"
		// * 若讀入normalmap texture
		// * 請針對每張材質依序額外添上反光度1~64(數字愈小愈亮)
		// * 於檔案Shiniess文件(若沒有請自行創建)
		drTextureSource( const char* textureList, const char* sourcePath = NULL );

		// * 載入一組cubemap
		// * 傳入依序為right, left, bottom, top, front, back的路徑陣列
		// * 若副檔名為bmp需標記true
		drTextureSource( std::string* cubemapList, bool isBmp = false );

		// * 輸入一個路徑讀取一張2D材質
		drTextureSource( const char* sourcePath );
		~drTextureSource();
	};
}

#endif