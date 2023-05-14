MAIN = main
COMMONS = commons
EULER = euler

DEBUG = -g3 -O0
MD = mkdir
LIBS = -l$(COMMONS) -l$(EULER) -lm

#CFLAGS =  -Wall -Wextra -Werror
CPPFLAGS = -I sources -MP -MMD
GCC = gcc
OBJ = objects
LIB = libraries
SRC = sources
C = c
DLIB = -Wl,-rpath,libraries/

# main
MAIN_EXE = $(MAIN).exe
# статические либы.
EULER_LIB = $(LIB)/lib$(EULER).so
COMMONS_LIB = $(LIB)/lib$(COMMONS).so

MAIN_SOURCES = $(shell find $(SRC)/$(MAIN) -name '*.$(C)')
MAIN_OBJECTS = $(MAIN_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

EULER_SOURCES = $(shell find $(SRC)/$(EULER) -name '*.$(C)')
EULER_OBJECTS = $(EULER_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

COMMONS_SOURCES = $(shell find $(SRC)/$(COMMONS) -name '*.$(C)')
COMMONS_OBJECTS = $(COMMONS_SOURCES:$(SRC)/%.$(C)=$(OBJ)/$(SRC)/%.o)

# создание объектных файлов.
$(OBJ)/%.o: %.c
	@echo "\r\033[0;36m\r"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -c -fPIC -o $@ $<

# make (Первая зависимость - выполняет всё.)
.PHONY: $(MAIN) 
$(MAIN): $(MD) $(MAIN_EXE)

# make all - форматирование, компиляция и запуск в одной команде!
.PHONY: all
all: format $(MAIN) run draw

# добавление папок для бинарников (bin & obj)
.PHONY: $(MD)
mkdir:
	@echo "\033[1;35m----СОЗДАНИЕ ПАПОК----"
	mkdir -p $(OBJ)/$(SRC)/$(MAIN)
	mkdir -p $(OBJ)/$(SRC)/$(EULER)
	mkdir -p $(OBJ)/$(SRC)/$(COMMONS)
	mkdir -p exp/results
	mkdir -p $(LIB)
	
# сборка программы со всеми библиотеками.
$(MAIN_EXE): $(MAIN_OBJECTS) $(COMMONS_LIB) $(EULER_LIB)
	@echo "\033[1;31m----СБОРКА программы----"
	$(GCC) $(CFLAGS) $(DEBUG) $(CPPFLAGS) -o $@ $^ -L$(LIB) $(LIBS) $(DLIB)

# создание статических библиотек.
$(COMMONS_LIB): $(COMMONS_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	$(GCC) -shared -o $@ $^
$(EULER_LIB): $(EULER_OBJECTS)
	@echo -n "\r\033[0;33m\r"
	$(GCC) -shared -o $@ $^

.PHONY: clean
clean:
	rm -rf $(LIB) $(OBJ) exp/results && rm -rf main.exe exp/data.dat
run:
	./$(MAIN_EXE)

draw:
	cd exp && sh draw.sh 
format:
	@echo "\033[1;32m----ФОРМАТИРОВАНИЕ ИСХОДНОГО КОДА----"
	find . -type f -name '*.[ch]' -not -path './thirdparty/*' | xargs clang-format --style GNU -i --verbose
