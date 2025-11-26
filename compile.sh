../ppcg matvec_large.cpp --target=cuda
nvcc matvec_large_host.cu  matvec_large_kernel.cu -o matvec_large_cuda

g++ matvec_large.cpp -o matvec_large_cpu



g++ -O3 \
    -fgraphite \
    -fgraphite-identity \
    -floop-nest-optimize \
    -floop-parallelize-all \
    -fopenmp \
    matvec_large.cpp -o matvec_large_graphite


# ../ppcg matvec_large.cpp --target=opencl
# gcc -std=c99 matvec_large_host.c ocl_utilities.c -lOpenCL -o matvec_large_cl