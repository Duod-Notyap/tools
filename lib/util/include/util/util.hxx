#ifndef INCLUDE_TEST_UTIL_H_
#define INCLUDE_TEST_UTIL_H_

#include <iostream>

template<typename T>
void printPointerBits(T *ptr) {
    char* data = reinterpret_cast<char*>(ptr);
   
    // Determine the number of bits in uintptr_t
    int numBits = sizeof(T)*8;

    std::cout << "Pointer address: " << ptr << " (hexadecimal)" << std::endl;
    std::cout << "Binary representation(" << sizeof(T)*8 << " bits): ";

    // Iterate from the most significant bit to the least significant bit
    for(int b = 0; b < sizeof(T); b++) {
        for (int i = 7; i >= 0; --i) {
            std::cout << ((*(data+b) >> i) & 1);
        }
    }
    std::cout << std::endl;
}

#endif  // INCLUDE_TEST_UTIL_H_
