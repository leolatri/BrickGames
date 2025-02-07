#include <gtest/gtest.h>

#include "../brick_game/snake/game.h"
#include "../gui/desktop/snake_game_ui.h"
using namespace s21;

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// Init
// state______________________________________________________________________________________________________
TEST(SnakeTest, Ex_1) {
  Game_snake game;
  SnakeGameUI ui(game);

  EXPECT_EQ(game.tetg->action, Static);
}

TEST(SnakeTest, Ex_2) {
  Game_snake game;
  SnakeGameUI ui(game);

  game.createApple();
  game.tetg->curSnake[0].x = game.tetg->apple.x;
  game.tetg->curSnake[0].y = game.tetg->apple.y;
  game.eatingApple();

  game.updateInfo();

  EXPECT_EQ(game.tetg->score, 1);
  EXPECT_EQ(game.tetg->level, 1);
  EXPECT_EQ(game.tetg->curSnake.size(), 5);
}

TEST(SnakeTest, Ex_3) {
  Game_snake game;
  SnakeGameUI ui(game);
  game.tetg->action = Pause;

  EXPECT_EQ((game.tetg->action == Static), 0);
}

TEST(SnakeTest, Ex_4) {
  Game_snake game;
  SnakeGameUI ui(game);
  cairo_t* cr;
  game.drawField(cr);
  game.drawInfoField(cr);

  EXPECT_EQ(game.tetg->curSnake[0].x, 11);
}

TEST(SnakeTest, Ex_5) {
  Game_snake game;
  SnakeGameUI ui(game);

  game.createSnake();
  game.tetg->action = Left;
  game.moveSnake();

  EXPECT_EQ(game.tetg->curSnake[0].y, 4);
}

TEST(SnakeTest, Ex_6) {
  Game_snake game;
  SnakeGameUI ui(game);
  cairo_t* cr;

  game.tetg->action = Pause;
  game.printPost(cr);

  EXPECT_EQ((game.tetg->action == Pause), 1);
}

TEST(SnakeTest, Ex_7) {
  Game_snake game;
  SnakeGameUI ui(game);
  cairo_t* cr;

  game.tetg->action = Static;
  game.tetgInfo->high_score = 100;
  game.tetg->high_score = 80;
  game.tetg->score = 200;
  ui.updateGame();

  EXPECT_EQ((game.tetg->action == Win), 1);
}

TEST(SnakeTest, Ex_8) {
  Game_snake game;
  SnakeGameUI ui(game);
  cairo_t* cr;
  game.tetg->high_score = 100;
  ui.setHighScore();
  ui.getHighScore();
  EXPECT_EQ(game.tetg->high_score, 100);
}

TEST(SnakeTest, Ex_9) {
  int argc = 0;
  char* argv[] = {nullptr};
  Game_snake game;
  SnakeGameUI ui(game);

  ui.initUI(argc, argv);

  EXPECT_NE(ui.drawing_area, nullptr);
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  EXPECT_NE(window, nullptr);

  const gchar* title = gtk_window_get_title(GTK_WINDOW(window));

  gtk_widget_destroy(window);
}

TEST(SnakeTest, Ex_10) {
  Game_snake game;
  SnakeGameUI ui(game);
  GdkEventKey event;
  event.keyval = GDK_KEY_w;

  ui.on_key_press(nullptr, &event, &ui);

  EXPECT_EQ(ui.game.tetg->action, Up);
}

TEST(SnakeTest, Ex_11) {
  Game_snake game;
  SnakeGameUI ui(game);
  ui.game.tetg->action = Up;
  GdkEventKey event;
  event.keyval = GDK_KEY_s;

  ui.on_key_press(nullptr, &event, &ui);

  EXPECT_EQ(ui.game.tetg->action == Down, 0);
}

TEST(SnakeTest, Ex_12) {
  Game_snake game;
  SnakeGameUI ui(game);
  GdkEventKey event;
  event.keyval = GDK_KEY_a;

  ui.on_key_press(nullptr, &event, &ui);

  EXPECT_EQ(ui.game.tetg->action, Left);
}

TEST(SnakeTest, Ex_13) {
  Game_snake game;
  SnakeGameUI ui(game);
  GdkEventKey event;
  event.keyval = GDK_KEY_d;

  ui.on_key_press(nullptr, &event, &ui);

  EXPECT_EQ(ui.game.tetg->action, Right);
}

TEST(SnakeTest, Ex_14) {
  Game_snake game;
  SnakeGameUI ui(game);
  GdkEventKey event;
  event.keyval = GDK_KEY_p;

  ui.on_key_press(nullptr, &event, &ui);
  EXPECT_EQ(ui.game.tetg->action, Pause);

  ui.lastAction = Up;
  ui.on_key_press(nullptr, &event, &ui);
  EXPECT_EQ(ui.game.tetg->action, Up);
}

// TEST(SnakeTest, Ex_15) {
//     Game_snake game;
//     SnakeGameUI ui(game);
//     GdkEventKey event;
//     event.keyval = GDK_KEY_q;

//     ui.on_key_press(nullptr, &event, &ui);

//     EXPECT_EQ(ui.game.tetg->action, Terminate);
// }

TEST(SnakeTest, Ex_15) {
  Game_snake game;
  SnakeGameUI ui(game);
  ui.lastAction = Up;
  GdkEventKey event;
  event.keyval = GDK_KEY_space;

  ui.on_key_press(nullptr, &event, &ui);

  EXPECT_EQ(ui.game.tetg->action, ui.lastAction);
}

TEST(SnakeTest, Ex_16) {
  Game_snake game;
  SnakeGameUI ui(game);

  cairo_surface_t* surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 600);
  cairo_t* cr = cairo_create(surface);

  gboolean result = ui.on_draw(nullptr, cr, &ui);

  EXPECT_EQ(result, FALSE);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}