#include"../RenderEngine/include/Scene.h"
#include"../RenderEngine/include/Renderer.h"
void Scene::createEntity(const string & name, const QMatrix4x4 & transform)
{
	EntityPtr entity(new Entity);
	entity->name = name;
	entity->transform = transform;
	entities.push_back(entity);
}
void Scene::renderScene(ViewportPtr viewport, CameraPtr camera)
{
	if (auto renderPtr = renderer.lock()) {
		renderPtr->setViewport(viewport);
		renderPtr->setViewMatrix(camera->getTransform().inverted());
		renderPtr->setProjectionMatrix(camera->getProjectionMat());
		renderPtr->render(entities);
	}
}
void Scene::addEntity(EntityPtr entity)
{
	entities.push_back(entity);
}
void Scene::removeEntity(EntityPtr entity)
{
	for (auto iter = entities.begin(); iter != entities.end(); ++iter) {
		if ((*iter).get() == entity.get()) {
			entities.erase(iter);
		}
	}
}
CameraPtr Scene::createCamera()
{
	CameraPtr camera(new Camera);
	camera->setScene(shared_from_this());
	cameras.push_back(camera);
	return camera;
}
