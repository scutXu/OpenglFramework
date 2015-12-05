#include"OpenGLWindow.h"
#include"../RenderEngine/include/ResourcePool.h"
#include"../RenderEngine/include/ObjLoader.h"
#include<QMouseEvent>
#include"postProcess.h"
OpenGLWindow::OpenGLWindow(QWidget * parent) : QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	QSurfaceFormat::setDefaultFormat(format);
	initPhysx();
	simulation.reset(new FluidSimulation(pxSdk, pxScene));
}
OpenGLWindow::~OpenGLWindow()
{
	cudaInter.unRegisterTexture(normalTex);
	cudaInter.unRegisterTexture(eyePosTex);
	shutdownPostProcess();
	shutdownPhysx();
}
void OpenGLWindow::initializeGL()
{
	makeCurrent();
	initializeOpenGLFunctions();
	cudaInter.init();

	renderer.reset(new Renderer);
	windowScene.reset(new Scene);
	textureScene.reset(new Scene);

	windowScene->renderer = renderer;
	textureScene->renderer = renderer;

	auto window = renderer->createRenderWindow(this);
	textureTarget = renderer->createRenderTexture(800, 800);


	windowCamera = windowScene->createCamera();
	textureCamera = textureScene->createCamera();
	textureCamera->setFrustum(1, 90.0f, 1, 500);

	QMatrix4x4 cameraTransform;
	cameraTransform.translate(0, 45, 45);
	cameraTransform.rotate(-45, 1, 0, 0);

	windowCamera->setTransform(cameraTransform);
	textureCamera->setTransform(cameraTransform);

	ViewportPtr winViewport(new Viewport);
	winViewport->backgroundColor = QVector4D(0, 0, 0, 1);
	window->addViewport(winViewport, windowCamera);

	ViewportPtr texViewport(new Viewport(0, 0, 800, 800));
	texViewport->backgroundColor = QVector4D(0, 0.5, 0, 1);
	textureTarget->addViewport(texViewport, textureCamera);

	ShaderProgramPtr shaderA(new QOpenGLShaderProgram);
	shaderA->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\Resources\\shaders\\texTest.vs");
	shaderA->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\Resources\\shaders\\texTest.fs");
	shaderA->link();

	ShaderProgramPtr shaderB(new QOpenGLShaderProgram);
	shaderB->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\Resources\\shaders\\pointSprite.vs");
	shaderB->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\Resources\\shaders\\pointSprite.fs");
	shaderB->link();



	PassPtr passA(new Pass);
	passA->polygonMode = GL_FILL;
	passA->setShaderProgram(shaderA);
	eyePosTex = ((RenderTexture *)(textureTarget.get()))->getTexture();
	normalTex.reset(new QOpenGLTexture(QOpenGLTexture::Target::Target2D));
	normalTex->setAutoMipMapGenerationEnabled(false);
	normalTex->create();
	normalTex->bind();
	normalTex->setFormat(QOpenGLTexture::TextureFormat::RGBA32F);
	normalTex->setSize(800, 800);
	normalTex->allocateStorage();
	passA->addTexture(normalTex, "normalMap");
	cudaInter.registerTexture(normalTex);
	cudaInter.registerTexture(eyePosTex);



	PassPtr passB(new Pass);
	passB->polygonMode = GL_FILL;
	passB->setShaderProgram(shaderB);

	MaterialPtr materialA(new Material);
	materialA->addPass(passA);

	MaterialPtr materialB(new Material);
	materialB->addPass(passB);


	MeshPtr particleMesh(new Mesh);
	fluidVertices.reset(new VertexData);
	fluidVertices->buf.create();
	fluidVertices->buf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
	fluidVertices->buf.bind();
	fluidVertices->buf.allocate(8000 * sizeof(float)* 3);
	fluidVertices->type = GL_FLOAT;
	fluidVertices->tupleSize = 3;
	fluidVertices->vertexCount = 8000;
	particleMesh->addVertexBuffer("pos", fluidVertices);
	particleMesh->primitiveType = GL_POINTS;

	MeshPtr meshA(new Mesh);
	ObjData data;
	readObjData("..\\Resources\\models\\plane.obj", &data);
	meshA->addVertexBuffer("pos", std::move(data.vertices), GL_FLOAT, 3);
	meshA->addVertexBuffer("uv", std::move(data.texCoords), GL_FLOAT, 2);
	meshA->primitiveType = GL_TRIANGLES;




	EntityPtr entityB(new Entity);
	entityB->setMaterial(materialB);
	entityB->setMesh(particleMesh);
	textureScene->addEntity(entityB);

	EntityPtr entityA(new Entity);
	entityA->setMaterial(materialA);
	entityA->setMesh(meshA); 
	windowScene->addEntity(entityA);

	timer.start(12, this);

	simulation->createFluid("..\\Resources\\particles\\water", "water");
	physx::PxMaterial* pxMaterial = pxSdk->createMaterial(0.5, 0.5, 0.5);
	physx::PxTransform planePos = physx::PxTransform(physx::PxVec3(0.0f), physx::PxQuat(1.3f, physx::PxVec3(0.0f, 0.0f, 1.0f)));
	physx::PxRigidStatic* plane = pxSdk->createRigidStatic(planePos);
	plane->createShape(physx::PxPlaneGeometry(), *pxMaterial);
	pxScene->addActor(*plane);

	physx::PxTransform planePos1 = physx::PxTransform(physx::PxVec3(-15.0f,0,0));
	physx::PxRigidStatic* plane1 = pxSdk->createRigidStatic(planePos1);
	plane1->createShape(physx::PxPlaneGeometry(), *pxMaterial);
	pxScene->addActor(*plane1);

	physx::PxTransform planePos2 = physx::PxTransform(physx::PxVec3(15.0f, 0, 0), physx::PxQuat(physx::PxPi, physx::PxVec3(0.0f, 0.0f, 1.0f)));
	physx::PxRigidStatic* plane2 = pxSdk->createRigidStatic(planePos2);
	plane2->createShape(physx::PxPlaneGeometry(), *pxMaterial);
	pxScene->addActor(*plane2);

	physx::PxTransform planePos3 = physx::PxTransform(physx::PxVec3(0.0f, 0, 15.0f), physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f)));
	physx::PxRigidStatic* plane3 = pxSdk->createRigidStatic(planePos3);
	plane3->createShape(physx::PxPlaneGeometry(), *pxMaterial);
	pxScene->addActor(*plane3);


	physx::PxTransform planePos4 = physx::PxTransform(physx::PxVec3(0.0f, 0, -15.0f), physx::PxQuat(-physx::PxHalfPi, physx::PxVec3(0.0f, 1.0f, 0.0f)));
	physx::PxRigidStatic* plane4 = pxSdk->createRigidStatic(planePos4);
	plane4->createShape(physx::PxPlaneGeometry(), *pxMaterial);
	pxScene->addActor(*plane4);

	initPostProcess();
}
void OpenGLWindow::resizeGL(int w,int h)
{
	float aspect = float(w) / float(h);
	windowCamera->setFrustum(aspect, 45.0f, 1.0f, 500.0f);
}
void OpenGLWindow::paintGL()
{
	renderer->updateAllRenderTargets();
}
void OpenGLWindow::mousePressEvent(QMouseEvent * e)
{
}
void OpenGLWindow::mouseMoveEvent(QMouseEvent * e)
{
}
void OpenGLWindow::mouseReleaseEvent(QMouseEvent * e)
{
}
void OpenGLWindow::setFramePerSecond(int framePerSecond)
{
	timer.stop();
	timer.start(10,this);
}
void OpenGLWindow::timerEvent(QTimerEvent * e)
{
	this->update();
	/*float * temp = new float[2560000];
	eyePosTex->bind();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (GLvoid *)temp);
	for (int i = 0; i < 640000; ++i) {
		float ttt;
		for (int j = 0; j < 4; ++j) {
			if (temp[i * 4] == 0) {
				break;
			}
			qDebug() << temp[i * 4 + j];
		}
	}
	return;*/
	updatePhysx();
	simulation->updateVertexBuffer("water", fluidVertices);
	auto normalArray = cudaInter.mapTexture(normalTex);

	auto eyePosArray = cudaInter.mapTexture(eyePosTex);

	invokePostProcessKernel(eyePosArray, normalArray);
	cudaInter.unMapTexture(normalTex);
	cudaInter.unMapTexture(eyePosTex);
	
}
void OpenGLWindow::initPhysx()
{
	pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pxAllocator, pxErrorCallback);
	pxSdk = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, physx::PxTolerancesScale());
	physx::PxSceneDesc pxSceneDesc(pxSdk->getTolerancesScale());
	pxSceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	pxSceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	pxSceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	pxScene = pxSdk->createScene(pxSceneDesc);

}
void OpenGLWindow::updatePhysx()
{
	pxScene->simulate(1 / 60.0f);
	pxScene->fetchResults(true);
}
void OpenGLWindow::shutdownPhysx()
{
	pxSdk->release();
	pxFoundation->release();
}