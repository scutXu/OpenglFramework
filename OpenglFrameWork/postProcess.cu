#include"device_functions.h"
#include "device_launch_parameters.h"
#include"helper_math.h"
#include<vector>
#include<cassert>
using std::vector;

//#define FILTER_WINDOW_RADIUS 3
//#define FILTER_WINDOW_DIAMETER 7
//#define FILTER_WINDOW_ELEMENT_COUNT 49


#define SIGMA 1
#define BLUR_DEPTH_IMPACT_FACTOR 0.2

surface<void, cudaSurfaceType2D> eyePosSurfaceRef;	//RGBA 32 F
surface<void, cudaSurfaceType2D> normalSurfaceRef;
surface<void, cudaSurfaceType2D> bluredEyePosSurfaceRef;

//__constant__ float filterWindow[FILTER_WINDOW_ELEMENT_COUNT];

cudaChannelFormatDesc channelDesc;


cudaArray * bluredEyePosTexMem;

__global__ void blurEyePosZKernel()
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;

	float4 eyePos;
	float4 eyePosNear;
	surf2Dread(&eyePos, eyePosSurfaceRef, x * 16, y);
	if (eyePos.z > -0.001f) {
		surf2Dwrite(make_float4(0), bluredEyePosSurfaceRef, x * 16, y);
		return;
	}

	int filterRadius = __fdividef(480.0f, -eyePos.z);
	filterRadius = min(filterRadius, 10);
	float bluredZ = 0.0f;
	float sum = 0.0f;
	int tx;
	int ty; 
	float dDepth;
	float filterWeight;
	for (int i = -filterRadius; i <= filterRadius; ++i) {
		tx = x + i;
		if (tx < 0 || tx >= 800) {
			continue;
		}
		for (int j = -filterRadius; j <= filterRadius; ++j) {
			ty = y + j;
			if (ty < 0 || ty >= 800) {
				continue;
			}
			surf2Dread(&eyePosNear, eyePosSurfaceRef, tx * 16, ty);
			dDepth = abs(eyePos.z - eyePosNear.z) * BLUR_DEPTH_IMPACT_FACTOR;
			/*float weight = (exp(-0.5 * (pow((i - filterRadius) / SIGMA, 2.0) + pow((j - filterRadius) / SIGMA, 2.0)))
				/ (2 * 3.1415 * SIGMA * SIGMA)) * exp(-dDepth * dDepth);*/
			filterWeight = exp(-dDepth * dDepth);
			sum += filterWeight;
			bluredZ += (eyePosNear.z * filterWeight);
		}
	}
	
	eyePos.z = __fdividef(bluredZ, sum);
	surf2Dwrite(eyePos, bluredEyePosSurfaceRef, x * 16, y);
}
__global__ void calculateNormalKernel()
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	

	float4 eyePos;
	float4 eyePosNear;
	float4 dx;
	float4 dx2;
	float4 dy;
	float4 dy2;

	dx.z = dx2.z = dy.z = dy2.z = 999999.0f;

	surf2Dread(&eyePos, bluredEyePosSurfaceRef, x * 16, y);
	if (eyePos.z > -0.001f) {
		surf2Dwrite(make_float4(0), normalSurfaceRef, x * 16, y);
		return;
	}

	if (x != 0) {
		surf2Dread(&eyePosNear, bluredEyePosSurfaceRef, (x - 1) * 16, y);
		dx = eyePosNear - eyePos;
	}
	if (x < 799) {
		surf2Dread(&eyePosNear, bluredEyePosSurfaceRef, (x + 1) * 16, y);
		dx2 = eyePosNear - eyePos;
	}
	if (abs(dx.z) > abs(dx2.z)) {
		dx = dx2;
	}

	if (y != 0) {
		surf2Dread(&eyePosNear, bluredEyePosSurfaceRef, x * 16, y - 1);
		dy = eyePosNear - eyePos;
	}
	if (y < 799) {
		surf2Dread(&eyePosNear, bluredEyePosSurfaceRef, x * 16, y + 1);
		dy2 = eyePosNear - eyePos;
	}
	if (abs(dy.z) > abs(dy2.z)) {
		dy = dy2;
	}

	float3 normal = normalize(cross(make_float3(dx), make_float3(dy)));

	surf2Dwrite(make_float4(normal), normalSurfaceRef, x * 16, y);
}
void initPostProcess()
{
	channelDesc.x = 32;
	channelDesc.y = 32;
	channelDesc.z = 32;
	channelDesc.w = 32;
	channelDesc.f = cudaChannelFormatKind::cudaChannelFormatKindFloat;

	auto error = cudaMallocArray(&bluredEyePosTexMem, &channelDesc, 800, 800,cudaArraySurfaceLoadStore);
	assert(error == cudaSuccess);



	//float filterWindowData[FILTER_WINDOW_ELEMENT_COUNT];
	//float weight = 1.0f / FILTER_WINDOW_ELEMENT_COUNT;

	//float sigma = 1;
	//float sum = 0.0;

	//int index = 0;
	//for (int i = 0; i < FILTER_WINDOW_DIAMETER; ++i) {
	//	for (int j = 0; j < FILTER_WINDOW_DIAMETER; ++j) {
	//		filterWindowData[index] = exp(-0.5 * (pow((i - FILTER_WINDOW_RADIUS) / sigma, 2.0) + pow((j - FILTER_WINDOW_RADIUS) / sigma, 2.0)))
	//			/ (2 * 3.1415 * sigma * sigma);
	//		sum += filterWindowData[index];
	//		++index;
	//	}
	//}
	//for (int i = 0; i < FILTER_WINDOW_ELEMENT_COUNT; ++i) {
	//	filterWindowData[i] /= sum;
	//}
	//cudaMemcpyToSymbol(filterWindow, filterWindowData, sizeof(float)* FILTER_WINDOW_ELEMENT_COUNT);
}

void invokePostProcessKernel(cudaArray * eyePosArray,cudaArray * normalArray)
{
	dim3 gridDim;
	dim3 blockDim;
	gridDim.x = gridDim.y = 25;
	gridDim.z = 1;

	blockDim.x = blockDim.y = 32;
	blockDim.z = 1;

	cudaError error = cudaBindSurfaceToArray(&eyePosSurfaceRef, eyePosArray, &channelDesc);
	assert(error == cudaSuccess);

	error = cudaBindSurfaceToArray(&normalSurfaceRef, normalArray, &channelDesc); 
	assert(error == cudaSuccess);

	error = cudaBindSurfaceToArray(&bluredEyePosSurfaceRef, bluredEyePosTexMem, &channelDesc);
	assert(error == cudaSuccess);

	blurEyePosZKernel << <gridDim, blockDim >> >();
	cudaDeviceSynchronize();
	calculateNormalKernel << <gridDim, blockDim >> >();
}

void shutdownPostProcess()
{
	cudaFreeArray(bluredEyePosTexMem);
}