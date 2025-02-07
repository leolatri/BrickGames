#ifndef __GAME_H__
#define __GAME_H__

#include <gtk/gtk.h>
#include <stdlib.h>

#include <cstdio>
#include <vector>

#define WIDTH 10
#define HEIGHT 20

/**
 * @brief Метод, упрощающий вывод текста на экран.
 *
 * @param context Указатель на объект типа cairo_t, который представляет
 * контекст рисования.
 * @param x Отступ по горизонтали.
 * @param y Отступ по вертикали.
 * @param size Размер шрифта.
 * @param ... Дополнительные переменные.
 */

#define draw_text(context, x, y, size, ...) \
  {                                         \
    cairo_set_font_size(context, size);     \
    cairo_move_to(context, x, y);           \
    {                                       \
      char buf[256] = "";                   \
      snprintf(buf, 256, __VA_ARGS__);      \
      cairo_show_text(context, buf);        \
    }                                       \
  }

// figure--------------------------------------------------------------------------------------------------------------------------
/**
 * @brief Структура, отвечающая за координаты блока (куба).
 */
typedef struct {
  int x;
  int y;
} Cub;

extern std::vector<Cub> snakeFigure;

// action------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief Перечисление, отвечающее за статус игры.
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Static,
  Over,
  Win
} UserAction_t;

/**
 * @brief Структура, отвечающая за все данные игры.
 */
typedef struct Game {
  int** field;
  std::vector<Cub> curSnake;
  Cub apple;
  UserAction_t action;

  int score;
  int speed;
  int countLine;
  int high_score;
  int level;
} Game;

// dopTablo----------------------------------------------------------------------------------------------------------------------------
/**
 * @brief Структура, отвечающая за данные информационного поля.
 */
typedef struct {
  int** field;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

namespace s21 {
/**
 * @brief Класс, содержащий все данные игры "Змейка".
 */
class Game_snake {
 public:
  Game_snake();
  ~Game_snake();

  void createSnake();
  void moveSnake();
  void createApple();
  void clearSnake();
  int valid();
  void eatingApple();
  void updateInfo();

  int** createField();
  int** createInfoField();
  void freeField();
  void drawField(cairo_t* cr);
  void drawInfoField(cairo_t* cr);
  void printPost(cairo_t* cr);

  const std::vector<Cub>& getSnake() const;
  const GameInfo_t& getGameInfo() const;

  GameInfo_t* tetgInfo;
  Game* tetg;
};
}  // namespace s21

#endif