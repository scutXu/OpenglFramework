#include"../RenderEngine/include/ResourcePool.h"
ResourcePool * ResourcePool::getInstance()
{
	static ResourcePool pool;
	return &pool;
}