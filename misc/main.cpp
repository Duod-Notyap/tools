struct a {
    using type = void;
};

template<typename T>
void test(typename T::type arg) {
    
}

int main() {
    test<a>(2);
}
