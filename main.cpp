#include <iostream>

#include "statistik.hpp"

int main(int argc, char** args) {
    switch (argc)
    {
    case 3:
        Analyze(args[2], "betterfile", args[1][0], 0.1);
        break;
    case 4:
        Analyze(args[2], args[3], args[1][0], 0.1);
        break;
    default:
        return -1;
    }
    return 0;
}
