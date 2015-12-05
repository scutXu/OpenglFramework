#include"../RenderEngine/include/Mesh.h"
#include<iostream>
using namespace std;
IndexData::IndexData() : buf(QOpenGLBuffer::IndexBuffer), size(0)
{
	
}
Mesh::Mesh() 
{
	initializeOpenGLFunctions();
	primitiveType = GL_TRIANGLES;
	patchSize = 3;
}
Mesh::~Mesh()
{
	for (auto iter = bufferMap.begin(); iter != bufferMap.end(); ++iter) {
		if (iter->second->buf.isCreated()) {
			iter->second->buf.destroy();
		}
	}
	if (indexBuf.buf.isCreated()) {
		indexBuf.buf.destroy();
	}
}
void Mesh::addVertexBuffer(const string & attribute, vector<float> & data,GLenum type,int tupleSize)
{
	auto iter = bufferMap.find(attribute);
	if (iter != bufferMap.end()) {
		#ifdef _DEBUG
		cerr << "error:try to insert duplicated attributes" << endl;
		#endif
		return;
	}
	VertexDataPtr vertexData(new VertexData);
	bool successed = vertexData->buf.create();
	vertexData->buf.bind();
	vertexData->buf.allocate((const void *)(&data[0]), sizeof(float)* data.size());
	vertexData->type = type;
	vertexData->tupleSize = tupleSize;
	vertexData->vertexCount = data.size() / tupleSize;
	bufferMap.insert(VertexDataMap::value_type(attribute, vertexData));
}
void Mesh::addVertexBuffer(const string & attribute, VertexDataPtr vData)
{
	bufferMap.insert(VertexDataMap::value_type(attribute, vData));
}
void Mesh::setIndexBuffer(vector<unsigned> & indices)
{
	if (!indexBuf.buf.isCreated()) {
		indexBuf.buf.create();
	}
	indexBuf.size = indices.size();
	indexBuf.buf.bind();
	indexBuf.buf.allocate((const void *)&indices[0], sizeof(unsigned)*indices.size());
}
void Mesh::draw(shared_ptr<QOpenGLShaderProgram> program)
{
	int vertexCount = 65532;
	for (auto iter = bufferMap.begin(); iter != bufferMap.end(); ++iter) {
		if (iter->second->vertexCount < vertexCount) {
			vertexCount = iter->second->vertexCount;
		}
		iter->second->buf.bind();
		program->enableAttributeArray(iter->first.c_str());
		program->setAttributeBuffer(iter->first.c_str(), iter->second->type, 0, iter->second->tupleSize);
	}
	if (primitiveType == GL_PATCHES) {
		glPatchParameteri(GL_PATCH_VERTICES, patchSize);
	}
	if (indexBuf.size > 0) {
		indexBuf.buf.bind();
		glDrawElements(primitiveType, indexBuf.size, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(primitiveType, 0, vertexCount);
	}
}