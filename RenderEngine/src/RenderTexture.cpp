#include"../RenderEngine/include/RenderTexture.h"
RenderTexture::RenderTexture(int width,int height)
{
	glGenFramebuffers(1, &framebuffer);
	glGenRenderbuffers(1, &depthBuffer);
	texture.reset(new QOpenGLTexture(QOpenGLTexture::Target::Target2D));

	

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);

	texture->setAutoMipMapGenerationEnabled(false);
	texture->create();
	texture->bind();
	texture->setFormat(QOpenGLTexture::TextureFormat::RGBA32F);
	texture->setSize(width, height);
	texture->allocateStorage();


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureId(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthBuffer);

}
shared_ptr<QOpenGLTexture> RenderTexture::getTexture()
{
	return texture;
}
RenderTexture::~RenderTexture()
{
	if (glIsRenderbuffer(depthBuffer)) {
		glDeleteRenderbuffers(1, &depthBuffer);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	texture->destroy();
	if (glIsFramebuffer(framebuffer)) {
		glDeleteFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	}
}
void RenderTexture::bindFramebuffer()
{
	static const GLenum fboBuffs[] = { GL_COLOR_ATTACHMENT0 };
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glDrawBuffers(1, fboBuffs);
}