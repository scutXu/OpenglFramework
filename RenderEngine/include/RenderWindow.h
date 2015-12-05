#pragma once
#include"../RenderEngine/include/RenderTarget.h"
#include<QOpenGLWidget>
class RenderWindow : public RenderTarget
{
public:
	RenderWindow(QOpenGLWidget * widget);
	~RenderWindow();
	virtual void bindFramebuffer();
private:
	QOpenGLWidget * widget;
};