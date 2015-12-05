#include"../RenderEngine/include/RenderWindow.h"
RenderWindow::RenderWindow(QOpenGLWidget * pWidget)
{
	widget = pWidget;
}
RenderWindow::~RenderWindow()
{

}
void RenderWindow::bindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, widget->defaultFramebufferObject());
	static const GLenum windowBuff[] = { GL_BACK_LEFT };
	glDrawBuffers(1, windowBuff);
}