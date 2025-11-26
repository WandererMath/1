#include <omp.h>
#include <iostream>



int main() {
    omp_set_dynamic(1);
    const int N = 1000;
    int a[N], b[N];

    // Initialize
    for(int i = 0; i < N; i++) a[i] = i;

    // Polyhedral-friendly loop
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < N; i++) {
        b[i] = a[i] * 2; // simple work
    }

    std::cout << b[500] << "\n";
    return 0;
}
