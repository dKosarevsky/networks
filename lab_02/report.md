# Компьютерные сети. Лабораторная работа № 2

### [Руководство к ЛР](https://networking-labs.ru/mod/assign/view.php?id=228) | [Код](https://github.com/dKosarevsky/networks/tree/main/lab_02/src)

## Часть I.

Создать клиент-серверное приложение на основе сетевых (системных) сокетов, транспортный протокол **UDP**. Использовать язык С./ C++

1. Клиент должен передавать серверу десятичное число (вводится с клавиатуры).
2. На сервере число выводится в десятичной, двоичной, шестнадцатеричной, восьмеричной системах счисления + системе счисления с основанием номер студента по списку. (При совпадении с имеющейся прибавить 1).

Сервер и клиент должны корректно обрабатывать начало и конец соединения.

### Выполнение программы
Компиляция
```bash
gcc server.c -o server
gcc client.c -o client
```

Запуск
```bash
./server 4455
./client 4455
```

Запуск UDP сервера: 

![](img/img.png)

Запуск клиента и ввод числа:

![](img/img_1.png)

Результат вычислений на сервере:

![](img/img_2.png)

## Часть II.

1. Создать клиент-серверное приложение на основе сетевых (системных) сокетов, транспортный протокол **TCP**. Использовать язык С./ C++
2. Клиент должен передать серверу имя файла в виде строки (вводится на стороне клиента с клавиатуры). Сервер должен передать файл с указанным именем клиенту (если в папке с сервером присутствует такой файл). 
3. Сервер должен вести лог, в который заносится каждый переданный файл и время передачи (через пробел).

Запуск TCP сервера:

![](img/img_3.png)

Запуск клиента и ввод имени файла:

![](img/img_4.png)

Сервер считывает файл, отправляет его клиенту и выводит на печать запись лога:

![](img/img_5.png)

Клиент получает файл от сервера и выводит на печать его содержимое:

![](img/img_6.png)

Если в папке отсутствует запрашиваемый файл, сервер выводит сообщение об ошибке:

![](img/img_7.png)