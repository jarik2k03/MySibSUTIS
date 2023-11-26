% Предикат для обработки имени и записи в другой файл
process_names(InputFile, OutputFile) :-
    open(InputFile, read, InputStream),  % Открываем входной файл для чтения
    open(OutputFile, write, OutputStream),  % Открываем выходной файл для записи
    process_lines(InputStream, OutputStream),  % Вызываем предикат для обработки данных
    close(InputStream),  % Закрываем входной файл
    close(OutputStream).  % Закрываем выходной файл

% Предикат для обработки строк
process_lines(Input, Output) :-
    read_line_to_string(Input, Line),  % Читаем строку из входного файла  
    Line \= end_of_file,  % Проверяем, не достигли ли конца файла
    process_name(Line, Processed),  % Вызываем предикат для обработки имени
    write(Output, Processed),  % Записываем обработанное имя в выходной файл
    write(Output, '\n'),  % Добавляем символ перевода строки
    process_lines(Input, Output).
process_lines(_, _).

% Предикат для обработки имени
process_name(FullName, Processed) :-
    atomic_list_concat([LastName, FirstName, MiddleName | _], ' ', FullName),  % Разделяем полное имя на части
    sub_atom(FirstName, 0, 1, _, FirstInitial),
    sub_atom(MiddleName, 0, 1, _, MiddleInitital),
    atomic_list_concat([LastName, ' ', FirstInitial, '. ', MiddleInitital ,'.'], '', Processed).  % Формируем обработанную запись

% process_names('2_5_in.txt', '2_5_out.txt'). % Выполнение задачи
