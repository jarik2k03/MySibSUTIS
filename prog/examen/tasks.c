#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

short ip_adress(char* adres) // a.Задача проверки корректности IP адреса узла
{
    short x = 0;
    short i = 0;
    short num = 0;
    short degmass[4]; // массив разрядов для чисел ip адреса
    short adrmass[4]; // массив чисел ip адреса для проверки корректности
    short degree = -1; // разряд числа
    short points = 0; // количество точек адреса
    while (*(adres + x))
    {
        degree++;
        x++;        
        if (*(adres + x) == '.' || *(adres + x) == '\0')
        {
            if (i > 4)
                return 1;
            
            degmass[i] = degree;
            // printf("%d", degmass[i]); // вывод кол-ва разрядов для чисел ip адреса
            degree = -1;
            i++;
            points++;
            x++;
        }
    }
    if (points != 4)
        return 1;

    printf("\n");
    i = 0;
    while (*adres)
    {
        for (degree = degmass[i]; degree >= 0; degree--) // подсчет числа, в будущем записавшегося в adrmass[]
        {
            num += (*(adres) - '0') * (pow(10, degree)); 
            // printf("%d ", num);
            *adres++;
        }
        adrmass[i] = num;
        // printf("%d ", adrmass[i]); // проверка перевода строки с адресом в числовой массив
        num = 0;
        i++;
        *adres++;

    }
    for (i = 0; i < 4; i++)
        if (adrmass[i] > 254 || adrmass[i] < 0 || adrmass[3] == 0)
            return 1;
    return 0;
}

short simple_spn1(char* s, char* allowed) // b.Определение префикса строки, состоящего только из допустимых символов
{
    short flag = 1;
    short x = 0, y = 0;
    while (*(s + x))
    {
        y = 0;
        // printf("\nCheck for: %c\n", *(s + x)); // проверка совпадения для опр. буквы 
        flag = 1;
        while (*(allowed + y))
        {
            // printf("%c", *(allowed + y)); // допустимые символы
            if ((*(s + x)) == (*(allowed + y)))
                flag = 0;         
            y++;
        }
        // printf("\nflag = %d\n", flag); // проверка значения флага
        if (flag) // если символ строки не нашел совпадение с allowed
            return flag;
        x++;
    }
    return 0; // если строка состоит из разрешенных символов
}

short simple_spn2(char* s, char* forbidden) // c.Определение префикса строки, не содержащего недопустимых символов
{
    short x = 0, y = 0;
    while (*(s + x))
    {
        y = 0;
        while (*(forbidden + y))
        {
            if ((*(s + x)) == (*(forbidden + y)))
                return 1; // если символ строки нашел совпадение с forbidden        
            y++;
        }
        x++;
    }
    return 0; // если строка состоит из разрешенных символов
}

char* ins_del(char* src, char* sub, int index, char mode)
{
    short i;
    int sublength = sizeof(sub);
    char* endmassiv = malloc(164);    
    if (mode == 'i')
    {
        for (i = 0; i < index; i++)
            *(endmassiv + i) = *(src + i); // копирование строки до индекса    
        for (i = index; i < index + sublength; i++)
            *(endmassiv + i) = *(sub + i - index - 1); // вставка нужной нам подстроки
        for (i = index; *(src + i) != '\0'; i++)
            *(endmassiv + i + sublength - 1) = *(src + i); // копирование строки после индекса   
    }
    if (mode == 'd')
    {
        for (i = 0; i < index; i++)
            *(endmassiv + i) = *(src + i); // копирование строки до индекса    
        for (i = index; *(src + i + 2) != '\0'; i++)
            *(endmassiv + i) = *(src + i + 2); // копирование строки после индекса    
    }
    return endmassiv;
}

short substr_search(char* str, char* sub) // e.Поиск подстроки в строке.
{
    short i, j;
    int sizestr = strlen(str) - 1; // длина строки
    int sizesub = strlen(sub) - 1; // длина подстроки
    int flag = 0; // флаг, отвечает за обнаружение несовпадений
    if (sizestr < sizesub) // ошибка если строка меньше подстроки
        return -2;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (flag == 1)
            return i - sizesub; // возвращает первое совпадение
        if (str[i] == sub[0] && sizesub == 1)
            return i; // если подстрока из одной буквы

        if (str[i] == sub[0]) // если увидел совпадение с первой буквой, запускает проверку с остальными
        {
            flag = 1;
            for (j = 1; sub[j] != '\0'; j++)
            {
                i++;
                if (str[i] != sub[j])
                {
                    flag = 0; // несовпадение
                    i += sizesub - j - 1; // прыжок через образец
                    break;
                }
                    
            }
            
        }
    }
    return -1;
}

void num_string(char* string, int number) // f.Перевод числа в строковое представление
{
    short minus = 0;
    int tempdeg; // изменяемый в ходе цикла разряд
    short deg; // разряд числа, счет слева
    int degree = 0, i;
    if (number < 0)
    {
        minus = 1;
        string[0] = '-';
    }
    for (i = 9; i > 0; i--) // предполагает, что число не меньше 9 разрядов, считает от самого большого
    {
        tempdeg = pow(10, i);
        if ((number % tempdeg) == number) // проверка на кол-во разрядов
            degree = i;
        else
            break;

        // printf("%d\n", degree);       
    }
    deg = degree;
    for (i = 0 + minus; i < degree + minus; i++)
    {
        tempdeg = pow(10, deg-1);
        // printf("%d\n", number / tempdeg);
        if (!minus)
            string[i] = ((number) / (tempdeg)) - 10 * ((number) / (tempdeg * 10)) + '0'; // убирание старших разрядов (34 - 30), (128 - 120)
        else
            string[i] = -(((number) / (tempdeg)) - 10 * ((number) / (tempdeg * 10))) + '0'; 
            // убирание старших разрядов (34 - 30), (128 - 120), но превращая отрицательное число в положительное
        deg--;
    }
    
}

void string_num(int* number, char* string) // g.Перевод символов строки в число.
{
    short minus = 0;
    if (string[0] == '-')
        minus = 1;    // если минус
    short i, deg;
    short degree = strlen(string) - minus; // разряд равен длине строки (без минуса)

    deg = degree; // разряд числа, счет слева
    short tempdeg; // изменяемый в ходе цикла разряд

    for (i = 0 + minus; i < degree + minus; i++)
    {
        tempdeg = pow(10, deg - 1);
        *number += (string[i] - '0') * tempdeg;
        // printf("N %d\n", *number);
        deg--;    
    }
    if (minus)
        *number = *number * (-1); // умножение на -1, если отрицательное
}

short token(char** toks, char* src, char delim) // h.Разбиение строки на поля и ее восстановление после разбиения.
{
    short pos = 0;
    short n = 0;
    short i;
    char* tok = malloc(10); // токен, который будет сохраняться в массиве строк toks с макс длиной 10 чаров
    for (i = 0; *(src + i) != '\0'; i++)
    {
        if(*(src + i) == delim) // видит разделитель и сохраняет результат в toks
        {
            *(tok + i - pos) = '\0';
            pos = i + 1;
            toks[n] = tok;
            // printf("Toks: %s in %d pos\n", toks[n], n);
            
            tok = malloc(10); // выделение новой памяти (опустошение токена для следующей записи)
            n++;
            continue;
        }
        // printf("%c = %c\n", *(tok + i - pos), *(src + i));

        *(tok + i - pos) = *(src + i); // запись токена
    }
    
    toks[n] = tok; // последний токен

    // printf("Toks: %s in %d pos\n", toks[n], n); 

    return n; // сколько всего вышло разбиений
}

void token_cncat(char* main, char* piece, char delim) // h.Разбиение строки на поля и ее восстановление после разбиения.
{
    short i;

    short lenmain = strlen(main); // длина массива
    short null_pos = lenmain; // позиция для простановки нуль-терминатора
    
    *(main + lenmain) = delim; // ставим разделитель после main
    for (i = 0; *(piece + i) != '\0'; i++)
    {

        *(main + i + lenmain + 1) = *(piece + i); // из за разделителя приходится смещаться на 1 элемент вправо 
        null_pos++;
    }
    *(main + null_pos + 1) = '\0';
}

void simple_cncat(char* main, char* piece) // i.Конкатенация строк.
{
    short i;
    short lenmain = strlen(main); // длина массива
    short null_pos = lenmain; // позиция для простановки нуль-терминатора
    for (i = 0; *(piece + i) != '\0'; i++)
    {
        *(main + i + lenmain) = *(piece + i);
        null_pos++;
    }
    *(main + null_pos) = '\0';
}

short simple_equal(char* first, char* second) // j.Проверка строк на совпадение с учетом регистра.
{
    short i;
    for (i = 0; *(first + i) != '\0' || *(second + i) != '\0'; i++)
        if (*(first + i) != *(second + i))
            return 1;
    return 0;
    
}

short strong_equal(char* first, char* second) // k.Проверка строк на совпадение без учета регистра.
{
    short i;
    for (i = 0; *(first + i) != '\0' || *(second + i) != '\0'; i++)
        if ((*(first + i) != *(second + i)) && (*(first + i) + ' ' != *(second + i)) && (*(first + i) != *(second + i) + ' ')) 
            return 1; // плюс пробел это нижний регистр
    return 0;
    
}

short alphabet(char (*list)[100]) // k.Проверка строк на совпадение без учета регистра.
{
    short i, j, count = 9;
    char temp[100];
    for (i = 0; i < count; i++)
    {
        for (j = 0; j < count - i; j++)
        {
            if (strcmp(*(list + j), *(list + j + 1)) > 0)
            {
                strcpy(temp, *(list + j)); 
                strcpy(*(list + j), *(list + j + 1)); 
                strcpy(*(list + j + 1), temp);
                
            }
        }
    }
    return count;
}

void simple_cpy(char* pasted, char* copied) // m.Копирование строки.
{

    short i;
    while (*(copied + i) != '\0')
    {
        *(pasted + i) = *(copied + i); // копируем
        i++;
    }
    *(pasted + i) = '\0'; // вставляем нуль-терминатор
}

void to_snake(char * snake, char* camel) // n.Преобразование CamelCase в snake_case
{
    short move = 0; // кол-во сдвигов под нижнее подчеркивание
    short i;
    for (i = 0; *(camel + i) != '\0'; i++)
    {
        if (*(camel + i) >= 'A' && *(camel + i) <= 'Z')
        {
            *(snake + i + move) = '_'; // вставка нижнего подчеркивания 
            move++; // сдвиг после подчеркивания
            *(snake + i + move) = *(camel + i) + ' '; // понижение регистра разделяющей буквы
            continue;
        }
        *(snake + i + move) = *(camel + i); // копирование
    }
    
}

void to_camel(char * camel, char* snake) // o.Преобразование snake_case в CamelCase
{
    short move = 0;
    short i;
    for (i = 0; *(snake + i) != '\0'; i++)
    {
        
        if (*(snake + i) == '_') // анализирует нижнее подчеркивание и определяет смещение для camel
        {
            move++;
            continue;
        }
        
        if (*(snake + i - 1) == '_') 
        {
            
            *(camel + i - move) = *(snake + i) - ' '; // повышает регистр для буквы после подчеркивания
            continue;
        }
        
        *(camel + i - move) = *(snake + i); // копироваие
        
    }    
}

int main(int argc, char* argv[])
{
    if (argc != 2 || *argv[1] < 'a' || *argv[1] > 'q')
    {
        printf("Выбери задачу: \n");
        printf("a.Задача проверки корректности IP адреса узла.\n");
        printf("b.Определение префикса строки, состоящего только из допустимых символов.\n");
        printf("c.Определение префикса строки, не содержащего недопустимых символов.\n");
        printf("d.Вставка/удаление фрагмента строки.\n");
        printf("e.Поиск подстроки в строке.\n");
        printf("f.Перевод числа в строковое представление\n");
        printf("g.Перевод символов строки в число.\n");
        printf("h.Разбиение строки на поля и ее восстановление после разбиения.\n");
        printf("i.Конкатенация строк.\n");
        printf("j.Проверка строк на совпадение с учетом регистра.\n");
        printf("k.Проверка строк на совпадение без учета регистра.\n");
        printf("l.Сравнение строк по алфавиту.\n");
        printf("m.Копирование строки.\n");
        printf("n.Преобразование CamelCase в snake_case\n");
        printf("o.Преобразование snake_case в CamelCase\n");
        // printf("p.Подсчет суммы четных/нечетных чисел\n");
        // printf("q.Подсчет количества вхождений в строку символов из набора\n");
        return 0;
    }
    if (*argv[1] == 'a')
    {
        char adr[] = {"192.168.0.234"}; 
        if (!ip_adress(adr))
            printf("\nIp adress %s is correct!\n", adr);
        else
            printf("\nIp adress %s is not correct!\n", adr);
        return 0;
    }

    if (*argv[1] == 'b')
    {
        char welcome[] = {"welcome"}; 
        char check[] = {"ewcoml"}; 
        if (!simple_spn1(welcome, check))
            printf("\nString (%s) contains only allowed symbols (%s)\n", welcome, check);
        else
            printf("\nString (%s) contains not allowed symbols!\n", welcome);
        return 0;
    }
    
    if (*argv[1] == 'c')
    {
        char welcome[] = {"welcome"}; 
        char check[] = {"dqpk"}; 
        if (!simple_spn2(welcome, check))
            printf("\nString (%s) doesn`t contain forbidden symbols from: (%s)\n", welcome, check);
        else
            printf("\nString (%s) contains not allowed symbols!\n", welcome);
        return 0;
    }

    if (*argv[1] == 'd')
    {
        char* afterins;
        char* afterdel;
        
        char str[] = "qweyuiop"; 
        char substr[] = "rt"; 
        
        afterins = ins_del(str, substr, 3, 'i');
        printf("\nString after adding \nfor: %s \nsubstring: %s\n", str, substr);
        for (size_t i = 0; i < 16; i++)
            printf("%c", afterins[i]);
        printf("\n");

        afterdel = ins_del(str, substr, 5, 'd');
        printf("\nString after deleting \nfor: %s \nsubstring with length %ld\n", str, sizeof(substr) - 1);
        for (size_t i = 0; i < 16; i++)
            printf("%c", afterdel[i]);
        printf("\n");

        return 0;
    }
    if (*argv[1] == 'e')
    {
        short firstfind;
        char text[] = "cicicacacacada";
        char example[] = "ica";
        firstfind = substr_search(text, example);
        if (firstfind >= 0)
            printf("Found element on %d col\n", firstfind);
        else if (firstfind == -1)
            printf("Not found\n");
        else
            printf("Error!\n");
    }

    if (*argv[1] == 'f')
    {
        char* str = malloc(16);
        int num = -2889;
        num_string(str, num);
        printf("Your array: %s \n", str);
        free(str);
    }
    if (*argv[1] == 'g')
    {
        char str[] = "-702";
        int* num = malloc(sizeof(int));
        string_num(num, str);
        printf("Your number: %d \n", *num);
        free(num);
    }
    if (*argv[1] == 'h')
    {   
        short i;
        short power; // мощность множества (кол-во всех разбиений)
        char** strings = malloc(81920);
        char str[] = "185.2728.239.1986.try";
        char result[] = "";
        char del = '.';
        power = token(strings, str, del);

        for (i = 0; i < power + 1; i++)
            printf("\n%d string: %s\n", i + 1 , *(strings + i));
        

        token_cncat(result, *strings, del); // слипаем
        printf("Concatenation = %s\n\n", result);   
        for (i = 0; i < power + 1; i++)
        {
            token_cncat(result, *(strings + i), del);
            printf("Concatenation = %s\n", result);             
        }
        free(strings);
    }
    if (*argv[1] == 'i')
    {   
        char concat[] = "Виталий ";
        char enation[] = "Олегович!";
        simple_cncat(concat, enation);
        printf("Concatenation = %s\n", concat);
    }
    if (*argv[1] == 'j')
    {   
        short flag;
        char s1[] = "Москва";
        char s2[] = "москва";
        flag = simple_equal(s1, s2);
        if (!flag)
            printf("Strings: %s == %s\n", s1, s2);
        else
            printf("Strings: %s != %s\n", s1, s2);
    }
    if (*argv[1] == 'k')
    {   
        short flag;
        char s1[] = "Москва";
        char s2[] = "москва";
        flag = strong_equal(s1, s2);
        if (!flag)
            printf("Strings: %s == %s\n", s1, s2);
        else
            printf("Strings: %s != %s\n", s1, s2);
    }
    if (*argv[1] == 'l')
    {   
        short i;
        char names[10][100] = {"Petr", "Vladimir", "Alexander", "Rizvan", "Iosif", "Alexey", "Zahar", "Yaroslav", "Ayaal"};
        printf("Before: \n");
        for (i = 0; i < 10; i++)
            printf("%s\n", names[i]);
        alphabet(names);
        printf("After: \n");
        for (i = 0; i < 10; i++)
            printf("%s\n", names[i]);
    }
    if (*argv[1] == 'm')
    {   
        short flag;
        char s1[] = "bsdgfhhgfdhfghf";
        char s2[] = "Berlizov";
        printf("Strings: %s == %s\n", s1, s2);
        simple_cpy(s1, s2);
        
        printf("Strings: %s == %s\n", s1, s2);
    }
    if (*argv[1] == 'n')
    {
        
        char c[] = "toBeCpyString";
        char* s = malloc(32);
        to_snake(s, c);
        printf("Camel: %s\n", c);
        printf("Snake: %s\n", s);
        free(s);
    }
    if (*argv[1] == 'o')
    {
        
        char s[] = "to_be_string";
        char* c = malloc(32);
        to_camel(c, s);
        printf("Snake: %s\n", s);
        printf("Camel: %s\n", c);
        
        free(c);
    }
}