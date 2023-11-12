#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
namespace po = boost::program_options;

// формирование строк подсказки со значениями констант
#define HELP_STR_LEN(MIN,MAX) HELP_STR(length,MIN,MAX)
#define HELP_STR_NUM(MIN,MAX) HELP_STR(numbers,MIN,MAX)
#define HELP_STR(PARAM,MIN,MAX) "Password " #PARAM " from " #MIN " to " #MAX

// константы для длины пароля
#define DEFAULT_LEN 8
#define MIN_LEN 4
#define MAX_LEN 40

// константы для количества паролей
#define DEFAULT_NUM 1
#define MIN_NUM 1
#define MAX_NUM 20

// константы для алфавита
#define ALF_STD 1 // стандартный: буквы + цифры
#define ALF_PIN 0 // пинкод: только цифры
#define ALF_SEC 2 // усиленный: буквы + цифры + знаки

// шаблон класса для чисел с минимальным и максимальным ограничениями
template<int MIN, int MAX>
class limited {
private:
    int value;
public:
    limited(int v) : value(v) {}
    // проверка попадания в диапазон от MIN до MAX
    bool isValid() {
        return value >= MIN && value <= MAX;
    }
    // возврат значения
    int operator()() {
        return value;
    }
    // перегруженный вывод, необходим для корректной работы boost
    friend std::ostream& operator<<(std::ostream & o, const limited& m) {
        return o << m.value;
    }
};

// набор параметров генератора пароля
struct PassParams {
    limited<MIN_LEN, MAX_LEN> len {0}; // длина пароля
    limited<MIN_NUM, MAX_NUM> num {0}; // количество паролей
    int alf = ALF_STD; // алфавит пароля
    bool isCyr = false; // использовать кириллицу
    bool noConfl = false; // исключить похожие символы
} params;

// шаблон функции для перегрузки валидатора значений типа limited
template <int MIN, int MAX>
void validate(boost::any& v,
              const std::vector<std::string>& values,
              limited<MIN, MAX> * target_type, int)
{
    po::validators::check_first_occurrence(v);
    const std::string& s = po::validators::get_single_string(values);
    std::smatch m;
    if (std::regex_match(s, m, std::regex("\\d{1,2}"))) {
        limited<MIN, MAX> n(boost::lexical_cast<int>(m.str()));
        if (n.isValid()) {
            v = boost::any(n);
            return;
        }
    }
    throw po::invalid_option_value(s);
}

int main(int argc, char** argv) {
    try {
        // создание пустой коллекции ПКС
        po::options_description opts("Allowed options");
        // добавление параметров в коллекцию ПКС
        opts.add_options()
        ("help,h", "Show help")
        ("length,l",
         po::value<limited<MIN_LEN, MAX_LEN>>(&params.len)
         ->default_value(limited<MIN_LEN, MAX_LEN>(DEFAULT_LEN)),
         HELP_STR_LEN(MIN_LEN, MAX_LEN))
        ("numbers,n",
         po::value<limited<MIN_NUM, MAX_NUM>>(&params.num)
         ->default_value(limited<MIN_NUM, MAX_NUM>(DEFAULT_NUM)),
         HELP_STR_NUM(MIN_NUM, MAX_NUM))
        ("digits,d", "Generate pincode")
        ("symbols,s", "Generate strong password with symbols =+-@#$%^&*!")
        ("cyrillic", "use cyrillic chars instead of latin")
        ("no-conflict", "do not use chars of the same shape")
        ;
        // переменная для результатов парсинга
        po::variables_map vm;
        // парсинг
        po::store(po::parse_command_line(argc, argv, opts), vm);
        // присвоение значений по умолчанию
        po::notify(vm);

        // Выдать справку
        if (vm.count("help")) {
            std::cout << opts << "\n";
            exit(0);
        }
// ---- Параметры для генерации пароля ----
        // Если пинкод
        if (vm.count("digits")) {
            params.alf--;
        }
        // Если расширенный алфавит
        if (vm.count("symbols")) {
            params.alf++;
        }
        // проверка на совместное использование -s и -d
        if (vm.count("digits") && vm.count("symbols")) {
            throw po::error("Incompatible otions: '--symbols' and '--digits'");
        }
        // Если замена латиницы на кириллицу
        if (vm.count("cyrillic")) {
            params.isCyr = true;
        }
        // Если исключение символов с похожим начертанием
        if (vm.count("no-conflict")) {
            params.noConfl = true;
        }
        // если был пинкод, то игнорировать --cyrillic и --no-conflict
        if (params.alf == ALF_PIN) {
            params.isCyr = params.noConfl = false;
        }
// ----------------------------------------

// тестовый вывод декодированных параметров
        std::cout << "PassLen: " << params.len() << std::endl;
        std::cout << "NumPass: " << params.num() << std::endl;
        switch (params.alf) {
        case 0:
            std::cout << "Pincode\n";
            break;
        case 1:
            std::cout << "Standard Password\n";
            break;
        case 2:
            std::cout << "Password with simbols\n";
        }
        if (params.isCyr) {
            std::cout << "Use cyrillic symbols\n";
        }
        if (params.noConfl) {
            std::cout << "Not use conflicting symbols\n";
        }
// ToDo - здесь должен быть код генератора пароля

// Обработка ошибок
    } catch(po::error& e) {							   // ошибки декодирования ПКС
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        return 1;
    } catch(std::exception& e) {					   // другие известные исключения
        std::cerr << "error: " << e.what() << "\n";
        return 2;
    } catch(...) {                                     // неизвестные исключения
        std::cerr << "Exception of unknown type!\n";
        std::terminate();
    }
    return 0;
}
