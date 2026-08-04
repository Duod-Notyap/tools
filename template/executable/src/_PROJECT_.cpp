#include <iostream>
#include <string>

void help() {
    std::cerr << "__PROJECT__: <TODO>\n" 
        << "\n"
        << "Arguments:\n"
        << "\n"
        << "Options:\n"
        << "    -h | --help         Display this message" 
        << std::endl;
}

int main(int argc, char **argv) {
    for(int i = 1; i < argc; i++) {
        std::string arg{argv[i]};

        if(arg == "-h" || arg == "--help") {
            help();
            return 0;
        }
    }
}
