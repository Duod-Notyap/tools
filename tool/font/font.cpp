#include "ttf/ttf.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace ttf;

enum class FontCliAction {
    COPY,
    REMOVE_TABLE
};

int read(int argc, char **argv, int argi) {
    if(argc < argi + 1) {
        throw std::runtime_error{"Not enough options"};
    }

    std::string path = argv[argi];
    std::ifstream stream{path};

    ttf_file file;
    ttf_read(stream, &file);

    std::cout << file.header << '\n';
    for(int i = 0; i < file.header.numTables; i++) {
        std::cout << file.table_records[i] << '\n';
    }

    return 0;
}

int copy(int argc, char **argv, int argi) {
    if(argc < argi + 1) {
        throw std::runtime_error{"Not enough options"};
    }

    std::string path = argv[argi];
    std::ifstream stream{path};

    ttf_file file;
    ttf_read(stream, &file);

    std::ofstream out{"./copy.ttf"};
    file.write(out);

    return 0;
}

int remove(int argc, char **argv, int argi) {
    if(argc < argi + 2) {
        throw std::runtime_error{"Not enough options"};
    }

    std::string path = argv[argi];
    std::string table = argv[argi + 1];
    std::ifstream stream{path};

    ttf_file file;
    ttf_read(stream, &file);

    file.removeTable(table);

    std::ofstream out{"./copy.ttf"};
    file.write(out);

    return 0;
}

int main(int argc, char **argv) {
    std::string action = argv[1];

    if(action == "remove") {
        return remove(argc, argv, 2);
    } else if(action == "copy") {
        return copy(argc, argv, 2);
    } else if(action == "read") {
        return read(argc, argv, 2);
    } else {
        std::cerr << "Unrecognized action '" << action << "'" << std::endl;
        return 1;
    }

    return 0;
}
