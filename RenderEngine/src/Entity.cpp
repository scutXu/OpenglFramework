#include"../RenderEngine/include/Entity.h"
void Entity::setMaterial(MaterialPtr pMaterial)
{
	material = pMaterial;
}
MaterialPtr Entity::getMaterial()
{
	return material;
}
void Entity::setMesh(MeshPtr pMesh)
{
	mesh = pMesh;
}
MeshPtr Entity::getMesh()
{
	return mesh;
}
