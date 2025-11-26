export name="matvec_omp"
g++ $name.cpp -O3 -fopenmp -fgraphite -floop-nest-optimize -o $name

