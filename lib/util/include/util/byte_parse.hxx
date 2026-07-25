#include <istream>
#include <stdexcept>

template<typename T>
T reverse_bytes(const T* value)  {
    char buf[sizeof(T)];
    char *in = (char*)value;

    for(int i = 0; i < sizeof(T); i++) {
        buf[i] = in[sizeof(T)-i-1];
    }

    return *((T*)buf);
}

template<typename T>
T read(std::istream& in) {
    if(!in.good()) {
        throw std::runtime_error("Stream bad");
    }

    char buf[sizeof(T)];
    in.read(buf, sizeof(T));

    return *(T*)buf;
}

template<typename T>
T read_be(std::istream& in) {
    if(!in.good()) {
        throw std::runtime_error("Stream bad");
    }

    char buf[sizeof(T)];
    in.read(buf, sizeof(T));

    return std::byteswap(*(T*)buf);
}
