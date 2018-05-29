#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <inttypes.h>
#include <assert.h>
#include "utils.h"


#define GRIDSIZE 256
#define MAXGPU 1
#define MAXK 1024
#define MAXLOG 4096
#define LOCAL 16

uint32_t C[67108864/GRIDSIZE];


int main(){
    int N;
    uint32_t keyA, keyB;
    cl_int status;

    //platform
    cl_platform_id platform_id;
    cl_uint platform_id_got;
    status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
    assert(status == CL_SUCCESS && platform_id_got == 1);
    //printf("%d platform found\n", platform_id_got);


    //devices
    cl_device_id GPU[MAXGPU];
    cl_uint GPU_id_got;
    status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
    assert(status == CL_SUCCESS);
    //printf("There are %d GPU devices\n", GPU_id_got);


    //getcontext
    cl_context context = clCreateContext(NULL, 1, GPU, NULL, NULL, &status);
    assert(status == CL_SUCCESS);


    //commandqueue
    cl_command_queue commandQueue = clCreateCommandQueue(context, GPU[0], 0, &status);
    assert(status == CL_SUCCESS);


    //file
    FILE *kernelfp = fopen("vecdot.cl", "r");
    assert(kernelfp != NULL);
    char kernelBuffer[MAXK];
    const char *constKernelSource = kernelBuffer;
    size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);
    //printf("The size of kernel source is %zu\n", kernelLength);


    //program
    cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
    assert(status == CL_SUCCESS);
    status = clBuildProgram(program, 1, GPU, NULL, NULL, NULL);
    if (status != CL_SUCCESS){
        char errorLog[100000];
        clGetProgramBuildInfo(program, GPU[0], CL_PROGRAM_BUILD_LOG, sizeof(errorLog), errorLog, NULL);
        printf("%s", errorLog);
        //exit (0);
    }
    assert(status == CL_SUCCESS);
    //printf("Build program completes\n");


    //kernel
    cl_kernel kernel = clCreateKernel(program, "vecdot", &status);
    assert(status == CL_SUCCESS);
    //printf("Build kernel completes\n");

    //create buffer
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, 67108864/GRIDSIZE * sizeof(uint32_t) / LOCAL, C, &status);
    assert(status == CL_SUCCESS);
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &keyA, &keyB) != EOF) {
        uint32_t padding = 0;
        while (N%(GRIDSIZE*LOCAL)) {
            padding += encrypt(N, keyA) * encrypt(N, keyB);
            N++;
        }
        //memset(C, 0, sizeof(uint32_t)*67108864);
        //set arguments
        status = clSetKernelArg(kernel, 0, sizeof(cl_uint), (void *) &keyA);
        //assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *) &keyB);
        //assert(status == CL_SUCCESS);
        //assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufferC);
        //assert(status == CL_SUCCESS);
        //printf("Set kernel arguments completes\n");

        //set shape

        size_t globalThreads[] = {(size_t)(N/LOCAL)};
        size_t localThreads[] = {GRIDSIZE};
        status =
          clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL,
      			   globalThreads, localThreads,
      			   0, NULL, NULL);
        //assert(status == CL_SUCCESS);
        //printf("Kernel execution completes.\n");

        //clFinish(commandQueue);

        status = clEnqueueReadBuffer(commandQueue, bufferC, CL_TRUE, 0, (N/GRIDSIZE)/LOCAL* sizeof(cl_uint), C, 0, NULL, NULL);
        //assert(status == CL_SUCCESS);
        uint32_t sum = 0;
        for(int i = 0; i < (N/GRIDSIZE)/LOCAL; i++){
            sum+=C[i];
        }


        printf("%" PRIu32 "\n", sum-padding);

    }
    clReleaseMemObject(bufferC);
    clReleaseContext(context);	/* context etcmake */
    clReleaseCommandQueue(commandQueue);
    clReleaseProgram(program);
    clReleaseKernel(kernel);

    return 0;

}
