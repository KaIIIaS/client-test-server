#include <algorithm>
#include <locale>
#include <codecvt>
#include <iostream>
#include <functional>
#include "generator.h"


void pw_generator::makeAlf()
{
    if (noConfl) {
        if (isCyr) {
            final_abc = rus_nc;
            final_dig = dig_nc_r;
        } else {
            final_abc = eng_nc;
            final_dig = dig_nc;
        }
    } else {
        final_dig = dig;
        if (isCyr) {
            final_abc = rus;
        } else {
            final_abc = eng;
        }
    }
    set_abc.insert(final_abc.begin(), final_abc.end());
    set_dig.insert(final_dig.begin(), final_dig.end());
    set_sym.insert(sym.begin(), sym.end());
    switch (alf) {
    case ALF_SEC:
        work_alf += sym;
    case ALF_STD:
        work_alf += final_abc;
    case ALF_PIN:
        work_alf += final_dig;
    }
}

bool pw_generator::badPassword(const std::wstring& psw)
{
    using namespace std::placeholders;
    std::locale loc("ru_RU.utf-8");
    bool res = false;
    switch (alf) {
    case ALF_SEC:
        res = res || std::none_of(psw.begin(), psw.end(), std::bind(std::ispunct<wchar_t>, _1, loc));
    case ALF_STD:
        res = res || std::none_of(psw.begin(), psw.end(), std::bind(std::isalpha<wchar_t>, _1, loc));
    case ALF_PIN:
        res = res || std::none_of(psw.begin(), psw.end(), std::bind(std::isdigit<wchar_t>, _1, loc));
    }
    return res;
}

std::wstring pw_generator::getPassword(std::mt19937& gen)
{
    std::wstring psw;
    do {
        std::shuffle(work_alf.begin(), work_alf.end(), gen);
        psw = work_alf.substr(0, len);
    } while (badPassword(psw));
    return psw;
}

std::vector<std::string> pw_generator::operator()()
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    makeAlf();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::string> pwds;
    for (int i = 0; i < num; ++i) {
        pwds.emplace_back(converter.to_bytes(getPassword(gen)));
    }
    return pwds;
}
