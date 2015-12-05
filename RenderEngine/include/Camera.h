#pragma once
#include<QMatrix4x4>
#include<memory>
using std::weak_ptr;
using std::shared_ptr;
class Scene;
class Camera
{
public:
	void setFrustum(float pAspect,float pFov,float pNear,float pFar);
	void setTransform(const QMatrix4x4 & pTransform);
	void setScene(shared_ptr<Scene> scene);
	QMatrix4x4 getTransform()const;
	QMatrix4x4 getProjectionMat()const;
	weak_ptr<Scene> getScene();
private:
	QMatrix4x4 transform;
	QMatrix4x4 projection;
	weak_ptr<Scene> sceneRef;
};

typedef std::shared_ptr<Camera> CameraPtr;