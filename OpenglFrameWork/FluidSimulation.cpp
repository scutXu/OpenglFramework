#include"FluidSimulation.h"
#include<fstream>
#include<vector>
#include<sstream>
#include<QDebug>
using std::ifstream;
using std::stringstream;
FluidSimulation::FluidSimulation(physx::PxPhysics * pxSdk, physx::PxScene * pxScene) : pxSdkRef(pxSdk), pxSceneRef(pxScene)
{
	funcMap.insert(
		FuncMapType::value_type("GridSize", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setGridSize(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("MaxMotionDistance", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setMaxMotionDistance(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("RestOffset", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setRestOffset(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("ContactOffset", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setContactOffset(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("Damping", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setDamping(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("Restitution", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setRestitution(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("DynamicFriction", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setDynamicFriction(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("RestParticleDistance", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setRestParticleDistance(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("Viscosity", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setViscosity(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("Stiffness", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setStiffness(valueStr.toFloat());
	}))
		);

	funcMap.insert(
		FuncMapType::value_type("Damping", ([](physx::PxParticleFluid * fluid, string value){
		QString valueStr = QString::fromStdString(value);
		fluid->setDamping(valueStr.toFloat());
	}))
		);
}

void FluidSimulation::createFluid(const string & filePath, const string & name)
{
	ifstream file(filePath);
	if (!file) {
		return;
	}
	physx::PxU32 maxParticleNum;
	physx::PxVec3 fluidCenter;
	string line;
	string propertyName;
	string propertyValue;
	stringstream lineStream;
	physx::PxParticleCreationData pcd;

	std::getline(file, line);
	lineStream.str(line);
	lineStream >> maxParticleNum;
	physx::PxParticleFluid * fluid = pxSdkRef->createParticleFluid(maxParticleNum);
	
	std::getline(file, line);
	lineStream.clear();
	lineStream.str(line);
	lineStream >> fluidCenter.x;
	lineStream >> fluidCenter.y;
	lineStream >> fluidCenter.z;
	pcd.numParticles = maxParticleNum;



	while (!std::getline(file, line).eof()) {
		lineStream.clear();
		lineStream.str(line);
		lineStream >> propertyName;
		lineStream >> propertyValue;
		auto funcIter = funcMap.find(propertyName);
		if (funcIter != funcMap.end()) {
			funcIter->second(fluid, propertyValue);
		}
		else {
			
		}
	}


	float strip = fluid->getRestParticleDistance();
	physx::PxU32 * indexBuf = new physx::PxU32[maxParticleNum];
	physx::PxVec3 * posBuf = new physx::PxVec3[maxParticleNum];
	int numParticlePerLine = pow(maxParticleNum, 1 / 3.0f) + 0.5f;
	int numParticlePerPlane = numParticlePerLine * numParticlePerLine;
	for (int i = 0; i<maxParticleNum; ++i) {
		indexBuf[i] = i;
		posBuf[i].x = ((i % numParticlePerLine) - (numParticlePerLine / 2)) * strip;
		posBuf[i].y = ((i % numParticlePerPlane) / numParticlePerLine - (numParticlePerLine / 2)) * strip;
		posBuf[i].z = (i / numParticlePerPlane - (numParticlePerLine / 2)) * strip;
		posBuf[i] += fluidCenter;
	}
	pcd.indexBuffer = physx::PxStrideIterator<const physx::PxU32>(indexBuf);
	pcd.positionBuffer = physx::PxStrideIterator<const physx::PxVec3>(posBuf);
	bool successed = fluid->createParticles(pcd);
	delete[] indexBuf;
	delete[] posBuf;

	pxSceneRef->addActor(*fluid);
	fluidMap.insert(FluidMapType::value_type(name, fluid));
}
void FluidSimulation::updateVertexBuffer(const string & name, VertexDataPtr vertexData)
{
	static std::vector<float> tempBuf;
	auto fluidIter = fluidMap.find(name);
	if (fluidIter == fluidMap.end()) {
		return;
	}
	vertexData->buf.bind();
	physx::PxParticleReadData * rd = fluidIter->second->lockParticleFluidReadData();
	if (rd) {
		tempBuf.resize(vertexData->vertexCount * 3);
		physx::PxStrideIterator<const physx::PxParticleFlags> flagsIt(rd->flagsBuffer);
		physx::PxStrideIterator<const physx::PxVec3> positionIt(rd->positionBuffer);
		for (unsigned i = 0; i < rd->validParticleRange; ++i) {
			const physx::PxVec3 & pPos = *positionIt;
			tempBuf[3 * i] = pPos.x;
			tempBuf[3 * i + 1] = pPos.y;
			tempBuf[3 * i + 2] = pPos.z;
			++positionIt;
			++flagsIt;
		}
		rd->unlock();
		vertexData->buf.write(0, (const void *)(&tempBuf[0]), sizeof(float) * vertexData->vertexCount * 3);
	}
}
void FluidSimulation::createEmptyBox(const string & name, const physx::PxVec3 & center, const physx::PxVec3 halfSize, physx::PxMaterial * material, bool isStatic)
{
	using namespace physx;
	auto boxShape = pxSdkRef->createShape(PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z),*material);
	PxActor * boxActor;
	if (isStatic) {
		boxActor = pxSdkRef->createRigidStatic(PxTransform(center));
		((PxRigidStatic *)(boxActor))->attachShape(*boxShape);
	}
	else {
		boxActor = pxSdkRef->createRigidDynamic(PxTransform(center));
		((PxRigidDynamic *)(boxActor))->attachShape(*boxShape);
	}
	pxSceneRef->addActor(*boxActor);
	actorMap.insert(ActorMapType::value_type(name, boxActor));
}