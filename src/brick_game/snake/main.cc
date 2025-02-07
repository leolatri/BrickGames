#include "../../gui/desktop/snake_game_ui.h"
#include "game.h"
using namespace s21;

int main(int argc, char *argv[]) {
  Game_snake game;
  SnakeGameUI ui(game);
  ui.getHighScore();

  int speed = game.tetg->speed;

  ui.initUI(argc, argv);
  g_timeout_add(speed, SnakeGameUI::update_game, &ui);

  gtk_main();
  return 0;
}