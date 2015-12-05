#include"CUDAInteroperator.h"
void CUDAInteroperator::init()
{
	cudaGLSetGLDevice(0);
}
bool CUDAInteroperator::registerTexture(shared_ptr<QOpenGLTexture> glTex)
{
	cudaGraphicsResource * resource;
	auto error = cudaGraphicsGLRegisterImage(&resource, glTex->textureId(),GL_TEXTURE_2D, cudaGraphicsRegisterFlagsSurfaceLoadStore);
	if (error != cudaSuccess) {
		return false;
	}
	resources.insert(ResourceMap::value_type(glTex, resource));
	return true;
}
cudaArray * CUDAInteroperator::mapTexture(shared_ptr<QOpenGLTexture> glTex)
{
	auto iter = resources.find(glTex);
	if (iter == resources.end()) {
		return NULL;
	}
	cudaGraphicsResource * res = iter->second;
	cudaGraphicsResource_t reses[] = { res };
	auto error = cudaGraphicsMapResources(1, reses); 
	if (error != cudaSuccess) {
		return NULL;
	}
	cudaArray * dataPointer;
	error = cudaGraphicsSubResourceGetMappedArray(&dataPointer, res, 0, 0); 
	if (error != cudaSuccess) {
		return NULL;
	}
	return dataPointer;

}
bool CUDAInteroperator::unRegisterTexture(shared_ptr<QOpenGLTexture> glTex)
{
	auto iter = resources.find(glTex);
	if (iter == resources.end()) {
		return NULL;
	}
	cudaGraphicsResource * res = iter->second;
	cudaGraphicsResource_t reses[] = { res };
	auto error = cudaGraphicsUnmapResources(1, reses);
	if (error != cudaSuccess) {
		return false;
	}
	return true;
}
bool CUDAInteroperator::unMapTexture(shared_ptr<QOpenGLTexture> glTex)
{
	auto iter = resources.find(glTex);
	if (iter == resources.end()) {
		return false;
	}
	cudaGraphicsResource * res = iter->second;
	cudaGraphicsResource_t reses[] = { res };
	auto error = cudaGraphicsUnmapResources(1, reses);
	if (error != cudaSuccess) {
		return false;
	}
	return true;
}