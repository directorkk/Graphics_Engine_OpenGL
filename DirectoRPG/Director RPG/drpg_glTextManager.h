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
		// * �r��j�p
		int fontHeight;
		// * �r��ʲ�0~900
		int fontWeight;
		// * ����
		bool fontItalic;
		// * ���u
		bool fontUnderline;
		// * �R���u
		bool fontStrikeOut;
		vector3 loc;
		HFONT hFont;
		std::vector<std::wstring> fontType;
		int compare( wchar_t* fontName );
		int compare( std::wstring fontName );
	public:
		// * �w�]�r�鬰Courier New
		drTextManager();
		// * �ۭq��l�r��, ��J�r��W(���t���ɦW)
		drTextManager( wchar_t* fontName );
		// * �ۭq��l�r��, ��J�r��W(���t���ɦW)
		drTextManager( std::wstring fontName );
		~drTextManager();
	
		// * �ϥ�listFontType���^�w�n�����r��
		// * �öǤJ�}�Cindex��fontID
		bool bindFontType( int fontID );
		// * �ϥΦr��, ������J�r��W(���t���ɦW)
		bool bindFontType( wchar_t* fontName );
		// * �ϥΦr��, ������J�r��W(���t���ɦW)
		bool bindFontType( std::wstring fontName );
		// * �u�Ψ�x, y
		// * ���W����(0, 0)
		// * �k�U�����������e
		bool setTextPos( vector3 printPos );
		// * ���W����(0, 0)
		// * �k�U�����������e
		bool setTextPos( double printX, double printY );
		// * �]�m�r��j�p
		bool setTextHeight( int fontH );
		// * �]�m�r��u���ʲ�, 0~9
		bool setTextWeight( int fontW );
		// * ����, ���u, �R���u
		bool setTextModel( bool italic = false, bool underline = false, bool strikeout = false );

		// * �ϥ�glColor�����C��
		bool printf( const char* str, ... );
		// * �ϥ�glColor�����C��
		bool printf( std::string str, ... );
		
		// * �K�[�r��, ������J�r��W(���t���ɦW)
		int addFontType( wchar_t* fontName );
		// * �K�[�r��, ������J�r��W(���t���ɦW)
		int addFontType( std::wstring fontName );

		// * �N�w�n�����r��W�ٶǤJfontList
		void getListFontType( std::string* fontList, unsigned int sizeList );
		// * ���o�ثe�n���r��ƶq
		int getAmountFont();
		// * ���o�{�b�ϥΪ��r��W��
		std::string getFontType();

		int getFontHeight();
		int getFontWeight();
		bool getStateItalic();
		bool getStateUnderline();
		bool getStateStrikeOut();
	};
}

#endif