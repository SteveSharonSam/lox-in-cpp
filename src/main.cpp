#include <iostream>
#include <fstream>
#include <string>
#include "Lox.h"


void read_file(std::ifstream *file) {
    std::string in_line;
    int line_num = 0;
    while(std::getline(*file, in_line)) {
        ++line_num;
        std::cout << line_num << ": " << in_line << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage " << argv[0] << " <file>\n";
        return 1;
    }


    //file.seekg(0);
    //if (file.is_open()) {
    //    read_file(&file);
    //} else {
    //    std::cout << "Unable to open file " << argv[1];
    //    file.close();
    //    return 2;
    //}

    Lox lox = Lox();
    lox.runFile(argv[1]);

}
