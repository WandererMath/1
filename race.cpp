#include <iostream>
#include <thread>
#include <vector>


#define N 1000000
// Function executed by each thread
void increment_counter(int* counter) {
    for (int i = 0; i < N; ++i) {
        // This is a "read-modify-write" operation
        // 1. Read 'counter'
        // 2. Modify (increment by 1)
        // 3. Write new value back to 'counter'
        (*counter)++; // This line is not atomic!
    }
}


int main() {
    // Create two threads
    int counter = 0;
    std::thread t1(increment_counter, &counter);
    std::thread t2(increment_counter, &counter);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    std::cout << "Expected counter value: " << 2*N << std::endl; 
    std::cout << "Final counter value: " << counter << std::endl; 

    return 0;
}