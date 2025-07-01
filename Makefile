CC = g++ -std=c++20
CFLAGS = -Wall -Wextra -Werror -g
TEST_FLAGS = -lgtest -pthread --coverage

SRC = s21_matrix_oop.cpp
TEST_SRC = test_s21_matrix_oop.cpp

OBJ = s21_matrix_oop.o
LIB = s21_matrix.a
TEST = test_s21_matrix_oop

OS = $(shell uname)

all: $(LIB)

clean:
	@echo "Очистка проекта..."
	@rm -rf *.o *.a *.gcda *.gcno test_s21_matrix_oop report/
	@echo "Очистка завершена."

$(LIB): $(OBJ)
	@echo "Создание статической библиотеки $(LIB)..."
	@ar rcs $@ $<
	@echo "Библиотека создана."

$(OBJ): $(SRC)
	@echo "Компиляция объектного файла $(OBJ)..."
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Объектный файл скомпилирован."

test: $(LIB) $(TEST_SRC)
	@echo "Сборка и запуск тестов..."
	$(CC) $(CFLAGS) $(TEST_FLAGS) -c $(TEST_SRC) -o test_s21_matrix_oop.o
	$(CC) $(CFLAGS) $(TEST_FLAGS) -o $(TEST) $(OBJ) test_s21_matrix_oop.o $(TEST_FLAGS)
	@./$(TEST)
	@echo "Тесты завершены."

gcov_report: test
	@echo "Генерация отчёта покрытия тестов..."
	@lcov -t "test" -o test.info --no-external -c -d . --ignore-errors inconsistent
	@genhtml -o report/ test.info
	@if [ "$(OS)" = "Darwin" ]; then \
		open ./report/index.html; \
	else \
		xdg-open ./report/index.html || true; \
	fi
	@rm -rf test.info *.gcda *.gcno
	@echo "Отчёт создан в report/index.html"

.PHONY: all clean test gcov_report