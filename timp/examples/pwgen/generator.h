#pragma once
#include <string>
#include <vector>
#include <random>
#include <set>
#include "constants.h"

class pw_generator {
private:
    static constexpr std::wstring_view eng {L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"};
    static constexpr std::wstring_view eng_nc {L"QWERTYUPASDFGHJKLZXCVBNMqwertyuiopasdfghjkzxcvbnm"};
    static constexpr std::wstring_view dig {L"0123456789"};
    static constexpr std::wstring_view dig_nc {L"23456789"};
    static constexpr std::wstring_view dig_nc_r {L"1245789"};
    static constexpr std::wstring_view sym {L"=+-@#$%^&*!"};
    static constexpr std::wstring_view rus {L"ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁйцукенгшщзхъфывапролджэячсмитьбю"};
    static constexpr std::wstring_view rus_nc {L"ЙЦУКЕНГШЩХЪФЫВАПРЛДЖЭЯЧСМИТЬБЮЁйцукенгшщзхъфывапролджэячсмитью"};
    std::wstring work_alf = L"";
    std::wstring_view final_abc = L"";
    std::set<wchar_t> set_abc;
    std::wstring_view final_dig = L"";
    std::set<wchar_t> set_dig;
    std::set<wchar_t> set_sym;
    int len = DEFAULT_LEN; // длина пароля
    int num = DEFAULT_NUM; // количество паролей
    int alf = ALF_STD; // алфавит пароля
    bool isCyr = false; // использовать кириллицу
    bool noConfl = false; // исключить похожие символы
    void makeAlf();
    std::wstring getPassword(std::mt19937& gen);
    bool badPassword(const std::wstring& psw);
public:
    pw_generator() {}
    void setLen(const int l) {
        len = l;
    }
    void setNum(const int n) {
        num = n;
    }
    void setAlf(const int a) {
        alf = a;
    }
    void setCyr(const bool b) {
        isCyr = b;
    }
    void setConfl(const bool b) {
        noConfl = b;
    }
    std::vector<std::string> operator()();
};
