#include <iostream>

// Zadeklaruj funkcję asemblera, aby C++ wiedział, że istnieje
extern "C" {
    long long CalculateMath(long long a, long long b);
}

int main() {
    long long result = CalculateMath(10, 25);
    std::cout << "Wynik z Asemblera: " << result << std::endl;
    return 0;
}