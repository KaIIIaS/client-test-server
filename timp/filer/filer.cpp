#include <iostream>
#include <fstream>
#include <typeinfo>
#include <vector>
#include <map>
#include <string_view>
#include <system_error>

// Defined by makefile. Uncomment if compile without makefile or with own custom makefile
//#define CODE 32

#ifndef CODE
#error CODE not defined. Define CODE in makefile or uncomment in code
#endif

#if CODE==64
typedef uint64_t T;
#elif CODE==32
typedef uint32_t T;
#elif CODE==16
typedef uint16_t T;
#elif CODE==-64
typedef int64_t T;
#elif CODE==-32
typedef int32_t T;
#elif CODE==-16
typedef int16_t T;
#elif CODE==-4
typedef float T;
#elif CODE==-8
typedef double T;
#else
#error Type T not supported
#endif
const std::map<char, std::string_view> dict {{'f', "float"},
    {'d', "double"},
    {'s', "int16_t"},
    {'t', "uint16_t"},
    {'i', "int32_t"},
    {'j', "uint32_t"},
    {'l', "int64_t"},
    {'m', "uint64_t"},
};
int main(int argc, char **argv)
{
    uint32_t v_nums;
    std::clog<<"Тип данных: "<<dict.at(typeid(T).name()[0])<<std::endl;
    if (argc !=3 )
        throw std::system_error(1000,
                                std::system_category(),
                                "Неверное количество параметров, необходимо 2");
    std::ifstream fin(argv[1]);
    if (!fin)
        throw std::system_error(1001,
                                std::system_category(),
                                "Невозможно открыть файл для чтения");
    std::ofstream fout(argv[2]);
    if (!fout)
        throw std::system_error(1002,
                                std::system_category(),
                                "Невозможно создать файл для записи");
    fin >> v_nums;
    if (!fin)
        throw std::system_error(1002,
                                std::system_category(),
                                "Ошибка чтения");
    std::vector<std::pair<uint32_t, std::vector<T>>> vect(v_nums);
    for (auto &v: vect) {
        fin >> v.first;
        if (!fin)
            throw std::system_error(1002,
                                    std::system_category(),
                                    "Ошибка чтения");
        v.second.resize(v.first);
        for (auto &e: v.second) {
            fin >> e;
            if (!fin)
                throw std::system_error(1002,
                                        std::system_category(),
                                        "Ошибка чтения");
        }
    }


    fout.write((const char*)&v_nums, sizeof(v_nums));
    if (!fout)
        throw std::system_error(1003,
                                std::system_category(),
                                "Ошибка записи");

    for (auto v: vect) {
        fout.write((const char*)&v.first, sizeof(v.first));
        if (!fout)
            throw std::system_error(1003,
                                    std::system_category(),
                                    "Ошибка записи");
        std::clog << v.first <<':';
        for (auto e: v.second) {
            fout.write((const char*)&e, sizeof(e));
            if (!fout)
                throw std::system_error(1003,
                                        std::system_category(),
                                        "Ошибка записи");
            std::clog << ' ' << e;
        }
        std::clog <<  std::endl;
    }
    fout.close();
    return 0;
}
