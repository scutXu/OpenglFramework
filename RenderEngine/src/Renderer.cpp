#include<iostream>
#include"../RenderEngine/include/Renderer.h"
#include"../RenderEngine/include/Viewport.h"
#include"../RenderEngine/include/Scene.h"
#include<QOpenGLWidget>
using namespace std;
Renderer::Renderer()
{
	initializeOpenGLFunctions();
	vao.create();
	vao.bind();
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_DEPTH_TEST);
}
Renderer::~Renderer()
{
	vao.destroy();
}
void Renderer::setClearColor(const QVector4D & clearColor)
{
	glClearColor(clearColor.x(), clearColor.y(), clearColor.z(), clearColor.w());
}
void Renderer::render(const vector<EntityPtr> & renderQueue)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	QMatrix4x4 vp = projection * view;
	for (auto iter = renderQueue.begin(); iter != renderQueue.end(); ++iter) {
		auto entity = (*iter);
		auto material = entity->getMaterial();
		auto mesh = entity->getMesh();
		int passCount = material->getPassCount();
		for (int i = 0; i < passCount; ++i) {
			auto pass = material->getPassAt(i);
			if (lastPass != pass) {
				auto shader = pass->getShaderProgram();
				if (lastShader.get() != shader.get()) {
					shader->bind();
					lastShader = shader;
				}
				/*if (pass->enablePolygonOffsetFill) {
					glEnable(GL_POLYGON_OFFSET_FILL);
				}
				else {
					glDisable(GL_POLYGON_OFFSET_FILL);
				}
				if (pass->enablePolygonOffsetLine) {
					glEnable(GL_POLYGON_OFFSET_LINE);
				}
				else {
					glDisable(GL_POLYGON_OFFSET_LINE);
				}
				if (pass->enablePolygonOffsetFill || pass->enablePolygonOffsetLine) {
					glPolygonOffset(pass->polygonOffsetParams.x(), pass->polygonOffsetParams.y());
				}
				glLineWidth(pass->lineWidth);*/
				pass->bindUserUniforms();
				pass->bindTextures();
				glPolygonMode(pass->renderFace, pass->polygonMode);
				shader->setUniformValue("mvp", vp * entity->transform);
				shader->setUniformValue("mv", view * entity->transform);
				shader->setUniformValue("p", projection);
				mesh->draw(shader);
			}
		}
	}
}
void Renderer::setViewMatrix(const QMatrix4x4 & pView)
{
	view = pView;
}
void Renderer::setProjectionMatrix(const QMatrix4x4 & pProjection)
{
	projection = pProjection;
}
void Renderer::setViewport(ViewportPtr viewport)
{
	glViewport(viewport->x, viewport->y, viewport->width, viewport->height);
	glClearColor(viewport->backgroundColor.x(), viewport->backgroundColor.y(), viewport->backgroundColor.z(), viewport->backgroundColor.w());
}

RenderTargetPtr Renderer::createRenderTexture(int width,int height)
{
	RenderTargetPtr renderTarget(new RenderTexture(width, height));
	renderTargets.push_back(renderTarget);
	return renderTarget;
}
RenderTargetPtr Renderer::createRenderWindow(QOpenGLWidget * widget)
{
	RenderTargetPtr renderTarget(new RenderWindow(widget));
	renderTargets.push_back(renderTarget);
	return renderTarget;
}
void Renderer::updateAllRenderTargets()
{
	for (auto iter = renderTargets.begin(); iter != renderTargets.end(); ++iter) {
		if ((*iter)->isAutoUpdate()) {
			(*iter)->update();
		}
	}
}