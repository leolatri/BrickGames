CC = g++
FLAGS = -std=c++17 -fprofile-arcs -ftest-coverage -g
GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK_LIBS = `pkg-config --libs gtk+-3.0`
TEST_FLAGS = -I/usr/include/cairo -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/pixman-1 -lcairo
TEST_RUN = ./test_snake.out

CC_C = gcc -g
FLAGS_C = -Wall -Werror -Wextra -c
TETRIS_OBJS = ./brick_game/tetris/main.o ./brick_game/tetris/backend.o ./brick_game/tetris/frontend.o
NCURSES_LIBS = -lncurses

# INSTALL_DIR = /usr/local/bin
DOC_DIR = /usr/local/share/doc/your_project

all: run_snake run_tetris

snake: main_s.out

main_s.out: main.o snake_game_ui.o game.o
	@$(CC) $(FLAGS) $(GTK_FLAGS) -o main_s.out main.o snake_game_ui.o game.o $(GTK_LIBS) -lncurses

main.o: ./brick_game/snake/main.cc
	@$(CC) $(FLAGS) $(GTK_FLAGS) -c ./brick_game/snake/main.cc -o main.o

game.o: ./brick_game/snake/game.cc
	@$(CC) $(FLAGS) $(GTK_FLAGS) -c ./brick_game/snake/game.cc -o game.o

snake_game_ui.o: ./gui/desktop/snake_game_ui.cc
	@$(CC) $(FLAGS) $(GTK_FLAGS) -c ./gui/desktop/snake_game_ui.cc -o snake_game_ui.o

run_snake: snake
	@echo "Запускаю игру 'Змейка'..."
	@./main_s.out
	@echo "Выход из игры 'Змейка'."

test: snake_game_ui.o game.o
	@$(CC) $(FLAGS) $(GTK_FLAGS) ./tests/test.cc -o test_snake.out -lncurses -lgtest -lgtest_main -pthread snake_game_ui.o game.o $(GTK_LIBS) -lcairo
	@$(TEST_RUN) 
	@clear
	@$(TEST_RUN)

coverage: test
	@gcovr -r . --html --html-details -o coverage.html --exclude-branches-by-pattern '.*'

format:
	@clang-format -n ./tests/*.cc ./brick_game/snake/*.cc ./brick_game/snake/*.h ./gui/desktop/*.cc ./gui/desktop/*.h ./brick_game/tetris/*.c ./brick_game/tetris/*.h
	@clang-format -i ./tests/*.cc ./brick_game/snake/*.cc ./brick_game/snake/*.h ./gui/desktop/*.cc ./gui/desktop/*.h ./brick_game/tetris/*.c ./brick_game/tetris/*.h


# ===========================
# Сборка и запуск Тетриса
# ===========================

tetris: main_t.out

main_t.out: $(TETRIS_OBJS)
	@$(CC_C) -o main_t.out $^ $(NCURSES_LIBS)

%.o: %.c
	@$(CC_C) $(FLAGS_C) -c $< -o $@

run_tetris: tetris
	@echo "Запускаю игру 'Тетрис'..."
	@./main_t.out
	@echo "Выход из игры 'Тетрис'."

clean:
	@rm -rf *.o *.a *.txt *.out *.gcda *.gcno *.html ./brick_game/tetris/*.o ./build/*.log ./build/*.out ./build/*.tex *.css *.gz
	@rm -rf build
	@rm -rf brick_game_dist brick_game_dist.zip dvi latex ./doc/html 
	clear

# ============================
# Скачивание, удаление, формат
# ============================

install: main_s.out main_t.out
	@echo "Установка исполняемых файлов..."
	@mkdir -p build
	@cp main_s.out build
	@cp main_t.out build
	@echo "Установка завершена."

uninstall:
	@echo "Удаление исполняемых файлов..."
	@rm -f build/snake
	@rm -f build/tetris
	@echo "Удаление завершено."

dvi:
	@cd ./doc && doxygen ./Doxyfile

dist:
	@rm -rf brick_game_dist brick_game_dist.zip
	@mkdir brick_game_dist
	@cp -rf brick_game brick_game_dist
	@cp -rf gui brick_game_dist
	@cp -rf tests brick_game_dist
	@cp -rf main.cc brick_game_dist
	@cp -rf Makefile brick_game_dist
	@7z a brick_game_dist.zip brick_game_dist
	@rm -rf brick_game_dist
	@echo "Создан архив по пути: " ${PWD}/brick_game_dist.zip


# dvi, перенести мэйн
