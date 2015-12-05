#pragma once
#include<QOpenGLFunctions_4_3_Core>
#include"../RenderEngine/include/Camera.h"
#include"../RenderEngine/include/Viewport.h"
#include<vector>
using std::vector;

typedef std::pair<ViewportPtr, CameraPtr> RenderArea;
class RenderTarget : protected QOpenGLFunctions_4_3_Core
{
public:
	RenderTarget();
	virtual ~RenderTarget() {}
	void update();
	void addViewport(ViewportPtr pViewport, CameraPtr pCamera);
	virtual void bindFramebuffer() = 0;
	ViewportPtr getViewport(int i)const;
	CameraPtr getCamera(int i)const;
	int getNumViewport()const;
	bool isAutoUpdate()const;
	void setAutoUpdate(bool isAuto);
protected:
	vector<RenderArea> renderAreas;
	bool autoUpdate;
};

typedef shared_ptr<RenderTarget> RenderTargetPtr;