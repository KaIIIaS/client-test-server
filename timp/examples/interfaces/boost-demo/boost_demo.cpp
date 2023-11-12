#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    std::string s;
    try {
        // создание пустой коллекции ПКС
        po::options_description opts("Allowed options"); // параметр - заголовок справки по коллекции ПКС
        // добавление параметров в коллекцию ПКС
        opts.add_options()
            ("help,h", "Show help") // парметр-переключатель - имя (длинное и короткое) и текст справки
            //дальше все параметры со значениями
            // параметр --first
            ("first", // имя (длинное)
             po::value<std::string>(&s), // значение типа std::string, сохраняется дополнительно в переменную s
             "First option is string")   // текст справки
            // параметр --second, -s
            ("second,s",                          // имя (длинное и короткое)
             po::value<int>()->default_value(25), //значение типа int, при умолчании = 25
             "Second option is int with default") // текст справки
            // параметр --third, -t
            ("third,t",                                // имя (длинное и короткое)
             po::value<float>()->implicit_value(3.14), //значение типа float, при пропуске = 3.14
             "Third option is float with implicit")    //текст справки
            // параметр --fourth, f
            ("fourth,f",                            // имя (длинное и короткое)
             po::value<std::vector<std::string>>(), //значение типа вектор из строк, дубликаты параметров
             "Fourth option is vector of string")   //текст справки
            // параметр --fifth
            ("fifth",                                     // имя (длинное)
             po::value<std::vector<int>>()->multitoken(), //значение типа вектор из целых, дубликаты значений
             "Fifth option is vector of int");            //текст справки

        // переменная для результатов парсинга
        po::variables_map vm;
        // парсинг
        po::store(po::parse_command_line(argc, argv, opts), vm);
        // присвоение значений по умолчанию
        po::notify(vm);
        // демонстрация
        // выдать справку по --help, -h
        if(vm.count("help")) {
            std::cout << opts << "\n";
            exit(0);
        }
        // first
        if(vm.count("first")) {
            std::cout << "option first: " << vm["first"].as<std::string>() << "\n";
            std::cout << s << " - as variable s\n";
        } else {
            std::cout << "option first not present\n";
        }
        // second
        if(vm.count("second")) {
            std::cout << "option second: " << vm["second"].as<int>() << "\n";
        } else {
            std::cout << "option second not present\n";
        }

        // third
        if(vm.count("third")) {
            std::cout << "option third: " << vm["third"].as<float>() << "\n";
        } else {
            std::cout << "option fthird not present\n";
        }

        // fourth
        if(vm.count("fourth")) {
            std::cout << "option fourth:";
            for(std::string s : vm["fourth"].as<std::vector<std::string>>()) {
                std::cout <<'\040'<< s;
            }
            std::cout << '\n';
        } else {
            std::cout << "option fourth not present\n";
        }
        
        // fifth
        if(vm.count("fifth")) {
            std::cout << "option fifth:";
            for(int n : vm["fifth"].as<std::vector<int>>()) {
                std::cout <<'\040'<< n;
            }
            std::cout << '\n';
        } else {
            std::cout << "option fifth not present\n";
        }
    } catch(po::error& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::cerr << "Use -h for help\n";
        return 1;
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 2;
    } catch(...) {
        std::cerr << "Exception of unknown type!\n";
        std::terminate();
    }
    return 0;
}
