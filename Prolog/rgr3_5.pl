:- consult('flightsDB.pl'). % Подключение файла с базой данных

% Просмотр содержимого базы данных
show_flights :-
    forall(flight(F, D, C), format('Flight ~w to ~w, Price: ~w~n', [F, D, C])).

% Добавление записи
add_flight(F, D, C) :-
    assert(flight(F, D, C)).

% Удаление записи
delete_flight(F) :-
    retract(flight(F, _, _)).

% Выполнение запроса к базе данных
find_flights_to(Dest, MinCost) :-
    flight(Flight, Dest, Cost),
    Cost < MinCost,
    format('Flight ~w to ~w, Price: ~w~n', [Flight, Dest, Cost]),
    fail.
find_flights_to(_, _).

% Сохранение базы данных в файл
save_to_file :-
    tell('flightsDB.pl'),
    listing(flight),
    told.

% Меню
menu :-
    repeat,
    write('1. Show flights\n'),
    write('2. Add a flight\n'),
    write('3. Delete a flight\n'),
    write('4. Find flights to destination with minimum cost\n'),
    write('5. Save and exit\n'),
    read(Choice),
    do_action(Choice),
    Choice == 5, !.

do_action(1) :- show_flights, !, fail.
do_action(2) :- write('Enter flight number: '), read(F), write('Enter destination: '), read(D), write('Enter price: '), read(C), add_flight(F, D, C), fail.
do_action(3) :- write('Enter flight number to delete: '), read(F), delete_flight(F), fail.
do_action(4) :- write('Enter the destination city: '), read(Dest), write('Enter the maximum price: '), read(MinCost), find_flights_to(Dest, MinCost), !, fail.
do_action(5) :- save_to_file.