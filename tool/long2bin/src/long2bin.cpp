#include <bit>
#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include "util/util.hxx"
#include "util/byte_parse.hxx"

void help() {
    std::cerr << "__PROJECT__: <TODO>\n" 
        << "\n"
        << "Arguments:\n"
        << "\n"
        << "Options:\n"
        << "    -h | --help         Display this message\n"
        << "    -u | --unsigned     Interpret as unsigned\n"
        << std::endl;
}

template<std::integral T>
T parse_integral(const std::string& value) {
    int64_t number;
    auto [ptr, ec] = std::from_chars<int64_t>(value.data(), value.data() + value.size(), number);

    if(ec == std::errc{} && ptr == value.data() + value.size()) {
        return number;
    }

    throw std::runtime_error{"Input '" + value + "' not in a valid format."};
}

void doSigned(const std::string& value) {
    auto number = parse_integral<int64_t>(value);

    if constexpr (std::endian::native == std::endian::little)
        number = reverse_bytes(&number);

    printBits(std::cout, number);
    std::cout << std::endl;
}

void doUnsigned(const std::string& value) {
    auto number = parse_integral<uint64_t>(value);

    if constexpr (std::endian::native == std::endian::little)
        number = reverse_bytes(&number);

    printBits(std::cout, number);
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    bool unsigned_ = false;
    std::optional<std::string> value;

    for(int i = 1; i < argc; i++) {
        std::string arg{argv[i]};

        if(arg == "-h" || arg == "--help") {
            help();
            std::exit(0);
        }

        if(arg == "-u" || arg == "--unsigned") {
            unsigned_ = true;
            continue;
        }

        if(value) {
            std::cerr << "Only one positional argument expected" << std::endl;
            std::exit(1);
        }

        value = arg;
    }

    if(!value) {
        std::cerr << "One positional argument expected" << std::endl;
        std::exit(1);
    }

    if(unsigned_) {
        doUnsigned(*value);
    } else {
        doSigned(*value);
    }
}
