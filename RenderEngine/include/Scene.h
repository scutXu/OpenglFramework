#pragma once
#include"../RenderEngine/include/Entity.h"
#include<vector>
#include<memory>
#include<QMatrix4x4>
#include"../RenderEngine/include/Camera.h"
#include"../RenderEngine/include/Viewport.h"
class Renderer;

using std::vector;
using std::shared_ptr;
using std::enable_shared_from_this;
typedef shared_ptr<Entity> EntityPtr;
typedef shared_ptr<QMatrix4x4> Mat4Ptr;
typedef vector<CameraPtr> CameraVector;
class Scene : public enable_shared_from_this<Scene>
{
public:
	void createEntity(const string & name = "Entity", const QMatrix4x4 & transform = QMatrix4x4());
	void addEntity(EntityPtr entity);
	void removeEntity(EntityPtr entity);
	void renderScene(ViewportPtr, CameraPtr camera);
	CameraPtr createCamera();
	std::weak_ptr<Renderer> renderer;
protected:
	vector<EntityPtr> entities;
	CameraVector cameras;
};
typedef shared_ptr<Scene> ScenePtr;