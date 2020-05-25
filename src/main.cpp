#include <fstream>
#include <iostream>

#include "menu.h"

int main() {
    std::ifstream ifs;
    ifs.open("maps/4x4/nodes.txt");

    if (ifs.fail()) {
        std::cerr << "Please set your working directory to point to the src/ folder!" << std::endl;
        return 1;
    }

    menu::menuLoop();
    return 0;
}
