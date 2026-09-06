#include <iostream>
#include <string>
#include "Lox.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage " << argv[0] << " <file>\n";
        return 1;
    }


    Lox lox = Lox();
    lox.runFile(argv[1]);

    return 0;
}
