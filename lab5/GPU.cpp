#include <fstream>
#include <iostream>
#include <time.h>

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define N 9
#define MAX_SOURCE_SIZE (0x100000)

int main(){
	clock_t t = clock();
	cl_int ret;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_platforms;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;

	/* получить доступные платформы */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	/* получить доступные устройства */
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	/* создать контекст */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* создаем команду */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	cl_program program = NULL;
	cl_kernel kernel = NULL;

	FILE *fp;
	const char fileName[] = "hello.cl";
	size_t source_size;
	char *source_str;

	try {
		fp = fopen(fileName, "r");
		if (!fp) {
			fprintf(stderr, "Failed to load kernel.\n");
			exit(1);
		}
		source_str = (char *)malloc(MAX_SOURCE_SIZE);
		source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
		fclose(fp);
	}
	catch (int a) {
		printf("%f", a);
	}

	/* создать бинарник из кода программы */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

	/* скомпилировать программу */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* создать кернел */
	kernel = clCreateKernel(program, "test", &ret);


	cl_mem memobj = NULL;
	cl_int* mem = (cl_int *)malloc(sizeof(cl_int));
	*mem = 0;

	/* создать буфер */
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int), NULL, &ret);
	/* записать данные в буфер */
	ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, sizeof(cl_int), mem, 0, NULL, NULL);
	/* устанавливаем параметр */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);


	size_t global_work_size[3] = { 362880 , 7, 7}; // 9! = 362800

	/* выполнить кернел */
	int i;
	for (i = 0; i < 1; i++)
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 3, NULL, global_work_size, NULL, 0, NULL, NULL);

	/* считать данные из буфера */
	ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, sizeof(float), mem, 0, NULL, NULL);

	printf("Found: %d\n", *mem);
	t = clock() - t;
	printf("It took me %d clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);

	getchar();
}
