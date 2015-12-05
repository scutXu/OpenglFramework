#ifndef ENTITY_H
#define ENTITY_H
#include<QMatrix4x4>
#include<string>
#include"Material.h"
#include"Mesh.h"
using std::string;

class Entity
{
public:
	QMatrix4x4 transform;
	string name;
public:
	void setMaterial(MaterialPtr pMaterial);
	MaterialPtr getMaterial();
	void setMesh(MeshPtr pMesh);
	MeshPtr getMesh();
protected:
	MaterialPtr material;
	MeshPtr mesh;
};

typedef shared_ptr<Entity> EntityPtr;

#endif // ENTITY_H
