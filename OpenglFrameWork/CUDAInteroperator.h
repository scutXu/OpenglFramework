#include"device_functions.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<Windows.h>
#include<gl\GL.h>
#include"cuda_gl_interop.h"
#include<QOpenGLtexture>
#include<memory>
#include<map>
using std::map;
using std::shared_ptr;

typedef map<shared_ptr<QOpenGLTexture>, cudaGraphicsResource *> ResourceMap;
class CUDAInteroperator
{
public:
	void init();
	bool registerTexture(shared_ptr<QOpenGLTexture> glTex);
	bool unRegisterTexture(shared_ptr<QOpenGLTexture> glTex);
	cudaArray * mapTexture(shared_ptr<QOpenGLTexture> glTex);
	bool unMapTexture(shared_ptr<QOpenGLTexture> glTex);
private:
	ResourceMap resources;
};