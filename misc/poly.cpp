#include <meta>
#include <iostream>

template<std::size_t N, typename T>
auto ptr_n(T& object) {
    if constexpr (N <= 0) {
        return object;
    } else {
        auto ptr = &object;
        return ptr_n<N-1>(ptr);
    }
}

struct Foo {};

int main() {
    Foo foo;

    auto ptr = ptr_n<10>(foo);
    std::cout << std::meta::display_string_of(std::meta::type_of(^^ptr)) << " " << ptr << std::endl;

    return 0;
}
