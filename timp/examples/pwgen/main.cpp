#include <iostream>
#include <string>
#include <vector>
#include "generator.h"
#include "interface.h"


int main(int argc, char ** argv)
{
    pw_generator gen;
    std::vector<std::string> pwds = interface(argc, argv, gen)();
    for (auto s: pwds) {
        std::cout << s << '\n';
    }
    return 0;
}
