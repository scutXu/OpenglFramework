#pragma once
#include "cuda_runtime.h"
void invokePostProcessKernel(cudaArray * eyePosArray, cudaArray * normalArray);
void initPostProcess();
void shutdownPostProcess();