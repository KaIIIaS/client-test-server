# Тестовый сервер для проверки клиентов

В сервере жестко заданы:

  * сетевой порт
  * имя клиента (единственное)
  * пароль клиента

## Сборка

 **`make`**

## Очистка

**`make clean`**

## Запуск

### Особенности работы

    1. Тип данных для сервера указывается через параметр -k при запуске
    2. Сервер запускается на порту 33333 на всех адресах компьютера
    3. При аутентификации сервер принимает только логин user и пароль P@ssW0rd

### Пример для типа uint32_t

 **`./server -k uint32_t`**


