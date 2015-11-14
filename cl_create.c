/*
 * cl_create.c
 *
 *  Created on: Mar 10, 2015
 *      Author: wing
 */
#include"cl_create.h"
#include<stdio.h>
#include<sys/stat.h>
static char * load_program_source(const char *filename) 
{
        struct stat statbuf;
        FILE *fh;
        char *source;
        fh = fopen(filename, "r");
        if (fh == 0)
                return 0;
        stat(filename, &statbuf);
        source = (char *) malloc(statbuf.st_size + 1); 
        fread(source, statbuf.st_size, 1, fh) ;
        source[statbuf.st_size] = '\0';
        return source; 
}
///
//  Create an OpenCL context on the first available platform using
//  either a GPU or CPU depending on what is available.
//
cl_context create_context(cl_device_type device_type) {
	cl_int errNum;
	cl_uint numPlatforms;
	cl_context context = NULL;
	cl_platform_id *platformId;

	// First, select an OpenCL platform to run on.  For this example, we
	// simply choose the first available platform.  Normally, you would
	// query for all available platforms and select the most appropriate one.
	errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
	platformId = (cl_platform_id*) malloc(
			sizeof(cl_platform_id) * numPlatforms);
	errNum = clGetPlatformIDs(numPlatforms, platformId, &numPlatforms);
	if (errNum != CL_SUCCESS || numPlatforms <= 0) {
		printf("Failed to find any OpenCL platforms.");
		return NULL;
	}

	// Next, create an OpenCL context on the platform.  Attempt to
	// create a GPU-based context, and if that fails, try to create
	// a CPU-based context.
	size_t i;
	for (i = 0; i < numPlatforms; i++) {
		cl_context_properties contextProperties[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties) platformId[i], 0 };
		context = clCreateContextFromType(contextProperties, device_type,
		NULL, NULL, &errNum);
		if (errNum == CL_SUCCESS)
			return context;

	}
		printf("Failed to find any OpenCL platforms with the given device type.");
	return NULL;
}

///
//  Create a command queue on the first device available on the
//  context
//
cl_command_queue create_command_queue(cl_context context,
		cl_device_id *device) {
	cl_int errNum;
	cl_device_id *devices;
	cl_command_queue commandQueue = NULL;
	size_t deviceBufferSize = -1;

// First get the size of the devices buffer
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL,
			&deviceBufferSize);
	if (errNum != CL_SUCCESS) {
		printf("Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)");
		return NULL;
	}

	if (deviceBufferSize <= 0) {
		printf("No devices available.");
		return NULL;
	}

// Allocate memory for the devices buffer
	devices =(cl_device_id*)malloc(deviceBufferSize);
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize,
			devices, NULL);
	if (errNum != CL_SUCCESS) {
		printf("Failed to get device IDs");
		return NULL;
	}

// In this example, we just choose the first available device.  In a
// real program, you would likely use all available devices or choose
// the highest performance device based on OpenCL device queries
	commandQueue = clCreateCommandQueue(context, devices[0],
	CL_QUEUE_PROFILING_ENABLE, NULL);
	if (commandQueue == NULL) {
		printf("Failed to create commandQueue for device 0");
		return NULL;
	}

	*device = devices[0];
	return commandQueue;
}

///
//  Create an OpenCL program from the kernel source file
//

///
//  Create an OpenCL program from the kernel source file
//
cl_program create_program(cl_context context, cl_device_id device,
		const char* srcFile) {
	cl_int errNum;
	cl_program program;
	char * source= load_program_source(srcFile);	
	if(!source)
	{
		printf("Error: Failed to load compute program from file!\n"); return NULL;
	}
	program = clCreateProgramWithSource(context, 1, (const char **)&source,
	NULL, NULL);
	if (program == NULL) {
		printf("Failed to create CL program from source.\n");
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS) {
// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
				sizeof(buildLog), buildLog, NULL);

		printf("Error in kernel: \n");
		printf("%s\n",buildLog);
		//std::cerr << buildLog;
		clReleaseProgram(program);
		return NULL;
	}

	return program;
}

