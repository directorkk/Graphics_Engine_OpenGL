#ifndef _DRPG_OPENGL_FRAMEBUFFEROBJECT_
#define _DRPG_OPENGL_FRAMEBUFFEROBJECT_

#include "drpg_header.h"
#include "drpg_glTextureSource.h"

namespace drpg
{
	class drFBO
	{
		unsigned int FBO;

		unsigned int createTexture( int w, int h, bool isDepth );
	public:
		std::vector<unsigned int> renderTex;
		std::vector<unsigned int> depthTex;
		// * �إߤ@�s��framebuffer object
		drFBO();
		// * �إߤ@�s��framebuffer object
		// * �P�ɫإߦ�m�����`�ק���U�@
		drFBO( int originTexWidth, int originTexHeight );
		~drFBO();
		// * ��framebuffer object�K�[����
		void addTexture( int w, int h, bool isDepth );

		void useFBO();
		void useFBO( unsigned int texIndex );
		void delFBO();


		// * ���oFBO��handle
		unsigned int getFramebufferID();
	};
}

#endif