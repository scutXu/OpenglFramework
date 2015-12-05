#ifndef RENDERER_H
#define RENDERER_H
#include<vector>
#include<QOpenGLFunctions_4_3_Core>  
#include <QOpenGLVertexArrayObject>
#include<QMatrix4x4>
#include"Entity.h"
#include"../RenderEngine/include/RenderTexture.h"
#include"../RenderEngine/include/RenderWindow.h"
using std::vector;

class Viewport;
class QOpenGLWidget;

typedef vector<RenderTargetPtr> RenderTargetVector;

class Renderer : protected QOpenGLFunctions_4_3_Core
{
public:
	Renderer();
	~Renderer();
	void render(const vector<EntityPtr> & renderQueue);
	void setViewMatrix(const QMatrix4x4 & pView);
	void setProjectionMatrix(const QMatrix4x4 & pProjection);
	void setClearColor(const QVector4D & clearColor);
	void setViewport(ViewportPtr viewport);
	void updateAllRenderTargets();
	RenderTargetPtr createRenderWindow(QOpenGLWidget * widget);
	RenderTargetPtr createRenderTexture(int width,int height);
protected:
	ShaderProgramPtr lastShader;
	PassPtr lastPass;

	QMatrix4x4 view;
	QMatrix4x4 projection;

	QOpenGLVertexArrayObject vao;
	RenderTargetVector renderTargets;
};

typedef shared_ptr<Renderer> RendererPtr;

#endif // RENDERER_H
