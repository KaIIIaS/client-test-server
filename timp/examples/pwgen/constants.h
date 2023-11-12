#pragma once

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
