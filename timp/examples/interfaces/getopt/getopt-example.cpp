#include <iostream>
#include <string>
#include <unistd.h>
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

    // атрибуты
    int passLen = defLen; // длина пароля
    int passNum = defNum; // количество паролей
    int passAlf = alfStd; // алфавит пароля

    // закрытые методы

    //вывод подсказки и останов
    // err - код ошибки. 0 - нет ошибки, 1-255 - номер ошибки
    void usage(unsigned err);
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
        std::cout << "Password with simbols =+-@#$%^&*!\n";
    }
    // TODO здесь будет сам генератор паролей
    return 0;
}

// реализация конструктора - весь разбор ПКС здесь
Opts::Opts(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "l:n:dsh")) != -1) {
        switch (opt) {
        case 'l':	// длина пароля
            passLen = std::stol(optarg);
            if (passLen < minLen || passLen > maxLen) {	// проверка на минимум и максимум
                std::cerr << "pass_len must be from " << minLen << " to " << maxLen << "\n";
                usage(1);
            }
            break;
        case 'n': 	// число паролей
            passNum = std::stol(optarg);
            if (passNum < minNum || passNum > maxNum) {	// проверка на минимум и максимум
                std::cerr << "pass_num must be from " << minNum << " to " << maxNum << "\n";
                usage(2);
            }
            break;
        case 'd':  	// пинкод
            passAlf--;
            if (passAlf != alfPin) {
                std::cerr << "Incompatible otions: -s and -d\n";
                usage(3);
            }
            break;
        case 's':	// усиленный алфавит
            passAlf++;
            if (passAlf != alfSec) {
                std::cerr << "Incompatible otions: -s and -d\n";
                usage(3);
            }
            break;
        case 'h': 	// справка
            usage(0);
            break;
        default:	// неизвесный параметр
            std::cerr << "Unknown otion\n";
            usage(4);
        }
    }
}

void Opts::usage(unsigned err) {
    std::cout << "Usage: genpass [-l pass_len] [-n pass_num] [-d]|[-s]\n";
    std::cout << "\t-l pass_len - password length from 4 to 40, default 8\n";
    std::cout << "\t-n num_pass - number of passwords from 1 to 20, default 1\n";
    std::cout << "\t-d - generate pincodes\n";
    std::cout << "\t-s - generate strong password with symbols =+-@#$%^&*!\n";
    // ненулевое значение говорит об ошибке
    exit(err);
}
