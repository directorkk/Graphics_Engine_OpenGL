#ifndef _DRPG_OPENGL_TEXT_MANAGE_
#define _DRPG_OPENGL_TEXT_MANAGE_

#include "drpg_header.h"

namespace drpg
{
	class drTextManager
	{
		unsigned int font;
		int fontUsed;
		bool setPos;

		bool loaded[65536];
		// * 字體大小
		int fontHeight;
		// * 字體粗細0~900
		int fontWeight;
		// * 斜體
		bool fontItalic;
		// * 底線
		bool fontUnderline;
		// * 刪除線
		bool fontStrikeOut;
		vector3 loc;
		HFONT hFont;
		std::vector<std::wstring> fontType;
		int compare( wchar_t* fontName );
		int compare( std::wstring fontName );
	public:
		// * 預設字體為Courier New
		drTextManager();
		// * 自訂初始字體, 輸入字體名(不含附檔名)
		drTextManager( wchar_t* fontName );
		// * 自訂初始字體, 輸入字體名(不含附檔名)
		drTextManager( std::wstring fontName );
		~drTextManager();
	
		// * 使用listFontType取回已登錄的字體
		// * 並傳入陣列index到fontID
		bool bindFontType( int fontID );
		// * 使用字體, 直接輸入字體名(不含附檔名)
		bool bindFontType( wchar_t* fontName );
		// * 使用字體, 直接輸入字體名(不含附檔名)
		bool bindFontType( std::wstring fontName );
		// * 只用到x, y
		// * 左上角為(0, 0)
		// * 右下角為視窗長寬
		bool setTextPos( vector3 printPos );
		// * 左上角為(0, 0)
		// * 右下角為視窗長寬
		bool setTextPos( double printX, double printY );
		// * 設置字體大小
		bool setTextHeight( int fontH );
		// * 設置字體線條粗細, 0~9
		bool setTextWeight( int fontW );
		// * 斜體, 底線, 刪除線
		bool setTextModel( bool italic = false, bool underline = false, bool strikeout = false );

		// * 使用glColor控制顏色
		bool printf( const char* str, ... );
		// * 使用glColor控制顏色
		bool printf( std::string str, ... );
		
		// * 添加字體, 直接輸入字體名(不含附檔名)
		int addFontType( wchar_t* fontName );
		// * 添加字體, 直接輸入字體名(不含附檔名)
		int addFontType( std::wstring fontName );

		// * 將已登錄的字體名稱傳入fontList
		void getListFontType( std::string* fontList, unsigned int sizeList );
		// * 取得目前登錄字體數量
		int getAmountFont();
		// * 取得現在使用的字體名稱
		std::string getFontType();

		int getFontHeight();
		int getFontWeight();
		bool getStateItalic();
		bool getStateUnderline();
		bool getStateStrikeOut();
	};
}

#endif