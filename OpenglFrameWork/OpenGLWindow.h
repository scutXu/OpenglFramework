#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include<QOpenGLFunctions_4_3_Core>  
#include<QOpenGLWidget>
#include<QMatrix4x4>
#include<QBasicTimer>
#include"../RenderEngine/include/Renderer.h"
#include"../RenderEngine/include/Scene.h"
#include<PxPhysicsAPI.h>
#include"FluidSimulation.h"
#include"CUDAInteroperator.h"
class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
public:
    explicit OpenGLWindow(QWidget *parent = 0);
    ~OpenGLWindow();
	void setFramePerSecond(int framePerSecond);
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent * e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	void timerEvent(QTimerEvent * e);
	void initPhysx();
	void updatePhysx();
	void shutdownPhysx();
private:
	QBasicTimer timer;
	RendererPtr renderer;
	ScenePtr windowScene;
	ScenePtr textureScene;
	physx::PxPhysics * pxSdk;
	physx::PxFoundation * pxFoundation;
	physx::PxDefaultErrorCallback pxErrorCallback;
	physx::PxDefaultAllocator pxAllocator;
	physx::PxScene * pxScene;

	VertexDataPtr fluidVertices;
	std::shared_ptr<FluidSimulation> simulation;
	CameraPtr windowCamera;
	CameraPtr textureCamera;
	RenderTargetPtr textureTarget;
	TexturePtr normalTex;
	TexturePtr eyePosTex;
	CUDAInteroperator cudaInter;
	TexturePtr testTex;
};
#endif // OPENGLWINDOW_H
