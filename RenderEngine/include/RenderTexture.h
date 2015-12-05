#pragma once
#include<QOpenGLTexture>
#include<memory>
#include"../RenderEngine/include/RenderTarget.h"

using std::shared_ptr;

class RenderTexture : public RenderTarget
{
public:
	RenderTexture(int width,int height);
	~RenderTexture();
	virtual void bindFramebuffer();
	shared_ptr<QOpenGLTexture> getTexture();
private:
	unsigned int framebuffer;
	unsigned int depthBuffer;
	shared_ptr<QOpenGLTexture> texture;
};

typedef shared_ptr<RenderTexture> RenderTexturePtr;