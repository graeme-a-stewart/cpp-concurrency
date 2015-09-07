#include <atomic>
#include <iostream>

int main() {
    std::atomic<bool> a_bool{true};
    std::atomic<int> a_int{0};
    std::atomic<long> a_long{0l};
    std::atomic<float> a_float{0.0f};
    std::atomic<double> a_double{0.0};

    std::cout << "Atomic bool is " << a_bool.is_lock_free() << std::endl;
    std::cout << "Atomic int is " << a_int.is_lock_free() << std::endl;
    std::cout << "Atomic long is " << a_long.is_lock_free() << std::endl;
    std::cout << "Atomic float is " << a_float.is_lock_free() << std::endl;
    std::cout << "Atomic double is " << a_double.is_lock_free() << std::endl;
}
