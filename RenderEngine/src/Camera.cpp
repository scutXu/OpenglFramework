#include"../RenderEngine/include/Camera.h"
#include"../RenderEngine/include/Scene.h"
void Camera::setFrustum(float pAspect, float pFov, float pNear, float pFar)
{
	projection.setToIdentity();
	projection.perspective(pFov, pAspect, pNear, pFar);
}
void Camera::setTransform(const QMatrix4x4 & pTransform)
{
	transform = pTransform;
}
void Camera::setScene(shared_ptr<Scene> pScene)
{
	sceneRef = pScene;
}
weak_ptr<Scene> Camera::getScene()
{
	return sceneRef;
}
QMatrix4x4 Camera::getTransform()const
{
	return transform;
}
QMatrix4x4 Camera::getProjectionMat()const
{
	return projection;
}
