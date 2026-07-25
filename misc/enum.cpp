#include <meta>
#include <iostream>

enum class Foo {
    A1,
    A2,
    A3
};

template<typename E> requires std::is_enum_v<E>
std::string_view enum_to_string(E value) {
    template for(constexpr auto v : std::define_static_array(std::meta::enumerators_of(^^E))) {
        if([: v :] == value) {
            return std::meta::identifier_of(v);
        }
    }

    return "";
}

int main() {
    std::string a { enum_to_string(Foo::A1) };
    std::cout << "String Value: " << a << std::endl;

    std::cout << "Values: " << std::endl;
    template for(constexpr auto e : std::define_static_array(std::meta::enumerators_of(^^Foo))) {
        std::cout << '\t' << std::meta::identifier_of(e) << std::endl;
    }
}

