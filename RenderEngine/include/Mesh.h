#ifndef MESH_H
#define MESH_H
#include<QOpenGLBuffer>
#include<QOpenGLShaderProgram>
#include<QOpenGLFunctions_4_3_Core>
#include<map>
#include<memory>
#include<string>
#include<vector>
using std::vector;
using std::shared_ptr;
using std::map;
using std::string;
struct VertexData {
	QOpenGLBuffer buf;
	int tupleSize;
	GLenum type;
	int vertexCount;
};
struct IndexData {
	IndexData();
	QOpenGLBuffer buf;
	int size;
};
typedef shared_ptr<VertexData> VertexDataPtr;
typedef map<string, VertexDataPtr> VertexDataMap;
class Mesh : protected QOpenGLFunctions_4_3_Core
{
public:
	Mesh();
	~Mesh();
	void addVertexBuffer(const string & attribute, vector<float> & data, GLenum type = GL_FLOAT,int tupleSize = 3);
	void addVertexBuffer(const string & attribute, VertexDataPtr vData);
	void setIndexBuffer(vector<unsigned> & indices);
	void draw(shared_ptr<QOpenGLShaderProgram> program);
public:
	GLenum primitiveType;
	int patchSize;
protected:
	VertexDataMap bufferMap;
	IndexData indexBuf;
};

typedef shared_ptr<Mesh> MeshPtr;

#endif // MESH_H


