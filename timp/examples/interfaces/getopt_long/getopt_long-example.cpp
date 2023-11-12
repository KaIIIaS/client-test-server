#include <iostream>
#include <string>
#include <getopt.h>
class Opts {
private:
    // константы с пределами значений
    static constexpr int minLen = 4;
    static constexpr int maxLen = 40;
    static constexpr int minNum = 1;
    static constexpr int maxNum = 20;

    // константы с значениями умолчаний
    static constexpr int defLen = 8;
    static constexpr int defNum = 1;

    // константы для алфавита
    static constexpr int alfStd = 1; // стандартный: буквы + цифры
    static constexpr int alfPin = 0; // пинкод: только цифры
    static constexpr int alfSec = 2; // усиленный: буквы + цифры + знаки

    // константы для длинных параметров, не имеющих короткой альтернативы
    static constexpr int parCyrillic = 1001;	// использовать кириллицу
    static constexpr int parNoConflict = 1002; // исключить похожие символы

    // массив для длинных параметров
    static const struct option long_options[];

    // атрибуты
    int passLen = defLen; // длина пароля
    int passNum = defNum; // количество паролей
    int passAlf = alfStd; // алфавит пароля
    bool passCyr = false; // использовать кириллицу
    bool passNoConfl = false; // исключить похожие символы

    // закрытые методы

    //вывод подсказки и останов
    // err - код ошибки. 0 - нет ошибки, 1-255 - номер ошибки
    void usage(unsigned err); //вывод подсказки и останов
public:
    // конструктор
    Opts(int argc, char **argv);
    // "геттеры" с результатами разбора параметров
    int getLen() {
        return passLen;    //длина пароля
    }
    int getNum() {
        return passNum;    //кол-во паролей
    }
    int getAlf() {
        return passAlf;    //алфавит пароля
    }
    bool isCyr() {
        return passCyr;   // используем кириллицу
    }
    bool noConflict() {
        return passNoConfl;   // исключаем похожие символы
    }
};

int main(int argc, char **argv) {
    Opts decoder(argc, argv); // реализуем разбор ПКС
    // получаем результат разбора ПКС
    std::cout << "PassLen: " << decoder.getLen() << std::endl;
    std::cout << "NumPass: " << decoder.getNum() << std::endl;
    switch (decoder.getAlf()) {
    case 0:
        std::cout << "Pincode\n";
        break;
    case 1:
        std::cout << "Standard Password\n";
        break;
    case 2:
        std::cout << "Password with simbols\n";
    }
    if (decoder.isCyr()) {
        std::cout << "Use cyrillic symbols\n";
    }
    if (decoder.noConflict()) {
        std::cout << "Not use conflicting symbols\n";
    }
    // TODO здесь будет сам генератор паролей
    return 0;
}

// заполнение массива для длинных параметров
const struct option Opts::long_options[] = {
    {"length",      required_argument, NULL, 'l'},
    {"numbers",     required_argument, NULL, 'n'},
    {"digits",      no_argument,       NULL, 'd'},
    {"symbols",     no_argument,       NULL, 's'},
    {"help",        no_argument,       NULL, 'h'},
    {"cyrillic",    no_argument,       NULL, Opts::parCyrillic},
    {"no-conflict", no_argument,       NULL, Opts::parNoConflict},
    {0, 0, 0, 0}
};

// реализация конструктора - весь разбор ПКС здесь
Opts::Opts(int argc, char **argv) {
    int opt;
    while ((opt = getopt_long(argc, argv, "l:n:dsh", long_options, NULL)) != -1) {
        switch (opt) {
        case 'l': 			// длина пароля
            passLen = std::stol(optarg);
            if (passLen < minLen || passLen > maxLen) {	// проверка на минимум и максимум
                std::cerr << "pass_len must be from " << minLen << " to " << maxLen << "\n";
                usage(1);
            }
            break;
        case 'n':			// число паролей
            passNum = std::stol(optarg);
            if (passNum < minNum || passNum > maxNum) {	// проверка на минимум и максимум
                std::cerr << "pass_num must be from " << minNum << " to " << maxNum << "\n";
                usage(2);
            }
            break;
        case 'd':			// пинкод
            passAlf--;
            if (passAlf != alfPin) {
                std::cerr << "Incompatible otions: -s and -d\n";
                usage(3);
            }
            break;
        case 's':			// со знаками
            passAlf++;
            if (passAlf != alfSec) {
                std::cerr << "Incompatible otions: -s and -d\n";
                usage(3);
            }
            break;
        case parCyrillic:	// замена на кириллицу
            passCyr = true;
            break;
        case parNoConflict:	// исключить похожие символы
            passNoConfl = true;
            break;
        case 'h':			// справка
            usage(0);
            break;
        default:			// неизвесный параметр
            std::cerr << "Unknown otion\n";
            usage(4);
        }
    }
    // если -d, то игнорировать --cyrillic и --no-conflict
    if (passAlf == alfPin) {
        passCyr = passNoConfl = false;
    }
}

void Opts::usage(unsigned err) {
    std::cout << "Usage: genpass [options]\n";
    std::cout << "\t-l | --length pass_len  - password length from 4 to 40, default 8\n";
    std::cout << "\t-n | --numbers num_pass - number of passwords from 1 to 20, default 1\n";
    std::cout << "\t-d | --digits           - generate pincodes\n";
    std::cout << "\t-s | --symbols          - generate strong password with symbols =+-@#$%^&*!\n";
    std::cout << "\t--cyrillic              - use cyrillic instead of latin\n";
    std::cout << "\t--no-conflict           - do not use chars of the same shape\n";
    // ненулевое значение говорит об ошибке
    exit(err);
}


