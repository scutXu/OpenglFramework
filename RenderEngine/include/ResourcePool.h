#pragma once
#include"Material.h"
#include"Mesh.h"
#include<map>
#include<string>
typedef std::map<std::string, PassPtr> PassMap;
typedef std::map<std::string, MeshPtr> MeshMap;
typedef std::map<std::string, TexturePtr> TextureMap;
typedef std::map<std::string, ShaderProgramPtr> ShaderProgramMap;
typedef std::map<std::string, MaterialPtr> MaterialMap;
class ResourcePool
{
public:
	MeshMap meshPool;
	TextureMap texturePool;
	ShaderProgramMap shaderPool;
	PassMap passPool;
	MaterialMap materialPool;
	static ResourcePool * getInstance();
private:
	ResourcePool() {}
	ResourcePool(const ResourcePool & pool) {}
	
};
#define ResourcePoolPtr ResourcePool::getInstance()
#define MeshPool ResourcePoolPtr->meshPool
#define TexturePool ResourcePoolPtr->texturePool
#define ShaderPool ResourcePoolPtr->shaderPool
#define PassPool ResourcePoolPtr->passPool
#define MaterialPool ResourcePoolPtr->materialPool