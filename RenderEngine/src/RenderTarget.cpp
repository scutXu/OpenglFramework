#include"../RenderEngine/include/RenderTarget.h"
#include"../RenderEngine/include/Scene.h"
RenderTarget::RenderTarget()
{
	initializeOpenGLFunctions();
	autoUpdate = true;
}
void RenderTarget::addViewport(ViewportPtr pViewport, CameraPtr pCamera)
{
	renderAreas.push_back(RenderArea::pair(pViewport, pCamera));
}
void RenderTarget::update()
{
	bindFramebuffer();
	for (auto iter = renderAreas.begin(); iter != renderAreas.end(); ++iter) {
		auto scene = iter->second->getScene().lock();
		if (scene) {
			scene->renderScene(iter->first, iter->second);
		}
	}
}
CameraPtr RenderTarget::getCamera(int i)const
{
	return renderAreas[i].second;
}
ViewportPtr RenderTarget::getViewport(int i)const
{
	return renderAreas[i].first;
}
int RenderTarget::getNumViewport()const
{
	return renderAreas.size();
}
bool RenderTarget::isAutoUpdate()const
{
	return autoUpdate;
}
void RenderTarget::setAutoUpdate(bool isAuto)
{
	autoUpdate = isAuto;
}