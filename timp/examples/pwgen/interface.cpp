#include <iostream>
#include <boost/program_options.hpp>
#include "constants.h"
#include "generator.h"

namespace po = boost::program_options;


pw_generator& interface(int argc, char**  argv, pw_generator& gen) {
    try {
        // создание пустой коллекции ПКС
        po::options_description opts("Allowed options");
        // добавление параметров в коллекцию ПКС
        opts.add_options()
        ("help,h", "Show help")
        ("length,l", po::value<int>(), HELP_STR_LEN(MIN_LEN, MAX_LEN))
        ("numbers,n", po::value<int>(), HELP_STR_NUM(MIN_NUM, MAX_NUM))
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

// ---- Проверки на недопустимые параметры ----

        // проверка на совместное использование -s и -d
        if (vm.count("digits") && vm.count("symbols")) {
            throw po::error("Incompatible otions: '--symbols' and '--digits'");
        }
        // проверка на минимум и максимум длины пароля и установка
        if (vm.count("length")) {
            int value = vm["length"].as<int>();
            if ( MIN_LEN <= value && value <= MAX_LEN)
                gen.setLen(value);
            else
                throw po::error("Wrong value in option '--length'");
        }
        // проверка на минимум и максимум количества паролей и установка
        if (vm.count("numbers")) {
            int value = vm["numbers"].as<int>();
            if ( MIN_NUM <= value && value <= MAX_NUM)
                gen.setNum(value);
            else
                throw po::error("Wrong value in option '--numbers'");
        }

// ---- Параметры для генерации пароля ----

        // Если пинкод
        if (vm.count("digits")) {
            gen.setAlf(ALF_PIN);
        } else {
            // Если замена латиницы на кириллицу
            if (vm.count("cyrillic")) {
                gen.setCyr(true);
            }
            // Если исключение символов с похожим начертанием
            if (vm.count("no-conflict")) {
                gen.setConfl(true);
            }
        }
        // Если расширенный алфавит
        if (vm.count("symbols")) {
            gen.setAlf(ALF_SEC);
        }


// ---- Обработка ошибок ----
    } catch(po::error& e) {								// ошибки декодирования ПКС
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        exit(1);
    } catch(std::exception& e) {						// другие известные исключения
        std::cerr << "error: " << e.what() << "\n";
        exit(2);
    } catch(...) {										// неизвестные исключения
        std::cerr << "Exception of unknown type!\n";
        std::terminate();
    }
    return gen;
}
