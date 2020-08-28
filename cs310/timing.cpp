#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>

void constant(int stopValue) {
    std::cout << "Hello!" << std::endl;
}
void linear(int stopValue) {
    for(int n = 0; n < stopValue; n++);
}
void quadratic(int stopValue) {
    for (int n = 0; n < stopValue; n++) {
        for (int m = 0; m < stopValue; m++) {

        }
    }
}

int runTest(const int &testSize, void (*f)(int)) {
    std::clock_t clockStart = std::clock();

    f(testSize);

    std::clock_t clockEnd = std::clock();
    int elapsed = (int) ((1000.0) * (clockEnd - clockStart) / CLOCKS_PER_SEC);
    return elapsed;
}

int main() {
    int test_size = 1000000;
    const int num_epochs = 5;
    for (int i = 0; i < num_epochs; i++) {
        std::cout << "Elapsed(" << test_size << "):";
        std::cout << runTest(test_size, linear) << std::endl;
        test_size <<= 1;
    }
}
