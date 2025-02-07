#ifndef __SNAKE_GAME_UI_H__
#define __SNAKE_GAME_UI_H__

#include "../../brick_game/snake/game.h"

namespace s21 {
/*Класс, содержащий все данные игры "Змейка" ui-части*/
class SnakeGameUI {
 public:
  SnakeGameUI(s21::Game_snake &game);
  void initUI(int argc, char *argv[]);
  void updateGame();
  void setHighScore();
  void getHighScore();
  UserAction_t lastAction;

  Game_snake &game;
  GdkEventKey *last_event;
  GtkWidget *window;
  GtkWidget *drawing_area;

  static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
  static gboolean update_game(gpointer user_data);
  static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event,
                               gpointer user_data);
};
}  // namespace s21

#endif
