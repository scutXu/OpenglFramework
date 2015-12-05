#pragma once
#include<QString>
#include<string>
#include<functional>
#include<map>
#include<PxPhysicsAPI.h>
#include"../RenderEngine/include/Mesh.h"
using std::map;
using std::function;
using std::string;

class FluidSimulation
{
public:
	FluidSimulation(physx::PxPhysics * pxSdk, physx::PxScene * pxScene);
	typedef function<void(physx::PxParticleFluid*, string)> FuncType;
	typedef map<std::string, FluidSimulation::FuncType> FuncMapType;
	typedef map<string, physx::PxParticleFluid *> FluidMapType;
	typedef map<string, physx::PxActor *> ActorMapType;
	void createFluid(const string & filePath,const string & name);
	void updateVertexBuffer(const string & name,VertexDataPtr vertexData);
	void createEmptyBox(const string & name,const physx::PxVec3 & center, const physx::PxVec3 halfSize,physx::PxMaterial * material,bool isStatic = true);
private:
	FuncMapType funcMap;
	FluidMapType fluidMap;
	ActorMapType actorMap;
	physx::PxPhysics * pxSdkRef;
	physx::PxScene * pxSceneRef;
};