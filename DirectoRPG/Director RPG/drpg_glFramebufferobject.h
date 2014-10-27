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
		// * 建立一新的framebuffer object
		drFBO();
		// * 建立一新的framebuffer object
		// * 同時建立色彩材質跟深度材質各一
		drFBO( int originTexWidth, int originTexHeight );
		~drFBO();
		// * 為framebuffer object添加材質
		void addTexture( int w, int h, bool isDepth );

		void useFBO();
		void useFBO( unsigned int texIndex );
		void delFBO();


		// * 取得FBO的handle
		unsigned int getFramebufferID();
	};
}

#endif