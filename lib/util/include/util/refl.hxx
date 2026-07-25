#include <meta>
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <any>

template<std::size_t N>
struct fixed_string {
    char data[N]{};

    consteval fixed_string(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }

    consteval operator std::string_view() const {
        return std::string_view(data, N-1);
    }
};

template<typename T, fixed_string S>
consteval std::meta::info get_property() {
    template for(constexpr auto member : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()))) {
        if constexpr (std::meta::identifier_of(member) == S) {
            return member;
        }
    }

    std::string err = "No property " + std::string{S} + " on object of type " + std::meta::display_string_of(^^T);
    throw std::invalid_argument{err};
}

template<fixed_string S, typename T>
[: std::meta::type_of(get_property<T, S>()) :] get_value(const T& value) {
    return value.[: get_property<T, S>() :];
}

template<typename T>
std::any get_value(const T& value, std::string name) {
    template for(constexpr auto member : std::define_static_array(std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()))) {
        if(std::meta::identifier_of(member) == name) {
            return value.[: member :];
        }
    }

    throw std::invalid_argument{"No property " + name + " on object of type " + std::meta::display_string_of(^^T)};
}

struct test_attr { int x = 3; };

template<typename T, typename Attr>
std::optional<Attr> get_attr() {
    constexpr auto vec = std::define_static_array(std::meta::annotations_of_with_type(^^T, ^^Attr));

    if constexpr (vec.size() > 0) {
        constexpr std::meta::info attr = vec.front();
        return std::meta::extract<Attr>(attr);
    } else {
        return std::nullopt;
    }
}

struct accessibility {
    static consteval accessibility Private() { return {.name = "private"}; };
    static consteval accessibility Public() { return {.name = "public"}; };
    static consteval accessibility Protected() { return {.name = "protected"}; };

    std::string_view name;

    bool operator==(const accessibility& other) {
        return name == other.name;
    }
};

template<std::meta::info v>
constexpr accessibility get_accessibility() {
    if constexpr (std::meta::is_public(v)) {
        return accessibility::Public();
    } else if constexpr (std::meta::is_private(v)) {
        return accessibility::Private();
    } else if constexpr (std::meta::is_protected(v)) {
        return accessibility::Protected();
    }

    throw std::invalid_argument{"Unknown Accessibility"};
}

template<typename T>
void print_obj(const T& value) {
    if(std::meta::is_structural_type(^^T)) {
        std::cout << "struct ";
    } else if(std::meta::is_class_type(^^T)) {
        std::cout << "class ";
    }
    std::cout << std::meta::display_string_of(^^T);

    bool first_base = true;
    template for(constexpr auto base : std::define_static_array(std::meta::bases_of(^^T, std::meta::access_context::unchecked()))) {
        if(first_base){ 
            std::cout << " : ";
            first_base = false;
        } else {
            std::cout << ", ";
        }

        std::cout << std::meta::display_string_of(std::meta::type_of(base));
    }

    std::optional<accessibility> last;
    std::cout << " {" << std::endl;
    template for(constexpr auto v : std::define_static_array(
        std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked())
    )) {
        constexpr auto access = get_accessibility<v>();
        if(!last.has_value() || *last != access) {
            std::cout << access.name << ":" << '\n';
            last = access;
        }

        std::cout << "\t" << std::meta::display_string_of(std::meta::type_of(v)) << " " << std::meta::identifier_of(v) << " = " << value.[:v:] << std::endl;
    }
    std::cout << "};" << std::endl;
}
