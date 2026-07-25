#include <iostream>
#include <system_error>

enum class test_error { 
    failure,
    other
};

struct test_error_category : public std::error_category {
    const char* name() const noexcept {        
        return "test_error";
    }

    std::string message(int ev) const noexcept {
        switch(static_cast<test_error>(ev)) {
        case test_error::failure:
            return "Some test message";
        }

        return "unknown";
    }
};

std::error_code make_error_code(const test_error& ec) noexcept {
    return {static_cast<int>(ec), test_error_category{}};
}

namespace std {
    template <>
    class is_error_code_enum<test_error> : public true_type {};
} /* std */

bool try_thing() {
    return false; // thing fails
}

void log_error(const std::error_code& ec) {
    if(ec == test_error::failure) {
        std::cout << "ec was failure " << ec.message() << std::endl;
    } else {
        std::cout << "ec was not failure " << ec.message() << std::endl;
    }
}

int main() {
    std::error_code ec;

    if(!try_thing()) {
        ec = test_error::failure;
    }

    log_error(ec);
    
    ec = test_error::other;
    log_error(ec);

    return 0;
}
