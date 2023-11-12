// C++
#include <iostream>
#include <string_view>
#include <set>
#include <system_error>

//POSIX
#include <unistd.h>

// self
#include "listener.h"
#include "worker.h"
#include "server_error.h"

std::string_view getVariant(int argc, char** argv);
void print_help_and_exit(int exit_code);
std::set<std::string_view> Variants = {
    "float",
    "double",
    "int16_t",
    "uint16_t",
    "int32_t",
    "uint32_t",
    "int64_t",
    "uint64_t"
};

int main(int argc, char **argv)
{
    try {
        Worker w(getVariant(argc, argv));
        Listener server;
        server.Run(w);
    } catch (std::system_error &e) {
        std::cerr << e.what() << std::endl;
        std::quick_exit(2);
    } catch (option_error &e) {
        std::cerr << e.what() << std::endl;
        print_help_and_exit(1);
    }
    return 0;
}

void print_help_and_exit(int exit_code)
{
    std::cerr << "Usage: server -k variant\n";
    std::cerr << "where variant is:\n";
    for (auto v: Variants) {
        std::cerr << '\t' << v << std::endl;
    }
    std::quick_exit(exit_code);
}

// user interface by getopt
std::string_view getVariant(int argc, char** argv)
{
    int opt;
    std::string_view variant = "";
    while( (opt = getopt(argc, argv, ":k:h")) != -1) {
        switch (opt) {
        case 'k' :
            variant = std::string_view(optarg);
            break;
        case 'h' :
            print_help_and_exit(0);
        case '?' :
            throw option_error(std::string("Option error: unknown option -") + (char)optopt);
        case ':' :
            throw option_error("Option error: missing variant");
        }
    }
    if (variant.empty())
        throw option_error("Option error: missing option");
    if (Variants.find(variant) == Variants.end())
        throw option_error("Otion error: unknown variant");
    return variant;
}



