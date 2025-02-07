#ifndef __SNAKE_GAME_UI_CC__
#define __SNAKE_GAME_UI_CC__

#include "./snake_game_ui.h"

#include <gtk/gtk.h>

using namespace s21;

// Конструктор класса SnakeGameUI, инициализирует ссылку на объект игры
SnakeGameUI::SnakeGameUI(Game_snake& game) : game(game) {
  window = nullptr;
  drawing_area = nullptr;
}

// Метод для обновления состояния игры
void SnakeGameUI::updateGame() {
  if (game.tetg->action != Pause && game.tetg->action != Win) {
    game.updateInfo();
    game.moveSnake();
    if (game.tetgInfo->score > game.tetg->high_score) {
      game.tetg->high_score = game.tetgInfo->score;
      if (game.tetg->score >= 200) game.tetg->action = Win;
      setHighScore();
    }
  }

  gtk_widget_queue_draw(drawing_area);
}

// Метод для сохранения рекорда в файл
void SnakeGameUI::setHighScore() {
  FILE* file = fopen("high_score.txt", "wb");
  fwrite(&game.tetg->high_score, sizeof(int), 1, file);
  fclose(file);
}

// Метод для получения рекорда из файла
void SnakeGameUI::getHighScore() {
  FILE* file = fopen("./high_score.txt", "rb");
  if (file == NULL) {
    game.tetg->high_score = 0;
    setHighScore();
  } else {
    size_t result = fread(&game.tetg->high_score, sizeof(int), 1, file);
    if (result != 1) {
      game.tetg->high_score = 0;
    }
    fclose(file);
  }
}

// Обработчик нажатий клавиш
gboolean SnakeGameUI::on_key_press(GtkWidget* widget, GdkEventKey* event,
                                   gpointer user_data) {
  SnakeGameUI* ui = static_cast<SnakeGameUI*>(user_data);
  if (ui->game.tetg->action != Pause) ui->lastAction = ui->game.tetg->action;

  switch (event->keyval) {
    case GDK_KEY_w:
    case GDK_KEY_W:
      if (ui->game.tetg->action != Down) {
        ui->game.tetg->action = Up;
      }
      break;
    case GDK_KEY_s:
    case GDK_KEY_S:
      if (ui->game.tetg->action != Up && ui->game.tetg->action != Static) {
        ui->game.tetg->action = Down;
      }
      break;
    case GDK_KEY_a:
    case GDK_KEY_A:
      if (ui->game.tetg->action != Right) {
        ui->game.tetg->action = Left;
      }
      break;
    case GDK_KEY_d:
    case GDK_KEY_D:
      if (ui->game.tetg->action != Left) {
        ui->game.tetg->action = Right;
      }
      break;
    case GDK_KEY_p:
    case GDK_KEY_P:
      if (ui->game.tetg->action != Pause) {
        ui->game.tetg->action = Pause;
      } else {
        ui->game.tetg->action = ui->lastAction;
      }
      break;
    case GDK_KEY_q:
    case GDK_KEY_Q:
      ui->game.tetg->action = Terminate;
      gtk_main_quit();
      break;
    case GDK_KEY_space:
      if (ui->game.tetg->action != Pause) {
        ui->game.tetg->action = ui->lastAction;
        ui->game.moveSnake();
      }
      break;
    default:
      break;
  }

  return FALSE;
}

// Метод для инициализации пользовательского интерфейса
void SnakeGameUI::initUI(int argc, char* argv[]) {
  gtk_init(&argc, &argv);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "SNAKE GAME");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), drawing_area);

  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), this);
  g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), this);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);

  gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) |
                                          GDK_BUTTON_PRESS_MASK);
  gtk_widget_show_all(window);
}

// Метод для отрисовки игрового поля и информации
gboolean SnakeGameUI::on_draw(GtkWidget* widget, cairo_t* cr,
                              gpointer user_data) {
  SnakeGameUI* ui = static_cast<SnakeGameUI*>(user_data);
  ui->game.drawField(cr);
  ui->game.drawInfoField(cr);

  if (ui->game.tetg->action == Pause || ui->game.tetg->action == Over ||
      ui->game.tetg->action == Win) {
    ui->game.printPost(cr);
  }

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_BOLD);

  draw_text(cr, 240, 30, 20, "GAME INFO");
  draw_text(cr, 210, 75, 15, "SCORE: %d", ui->game.tetgInfo->score);
  draw_text(cr, 210, 100, 15, "HIGH SCORE: %d", ui->game.tetgInfo->high_score);
  draw_text(cr, 210, 125, 15, "LEVEL: %d", ui->game.tetgInfo->level);
  draw_text(cr, 210, 150, 15, "SPEED: %d", ui->game.tetgInfo->speed);

  draw_text(cr, 210, 210, 15, "UP:        \'W\'");
  draw_text(cr, 210, 230, 15, "DOWN:  \'S\'");
  draw_text(cr, 210, 260, 15, "LEFT:      \'A\'");
  draw_text(cr, 210, 280, 15, "RIGHT:   \'D\'");
  draw_text(cr, 210, 310, 15, "END:       \'Q\'");
  draw_text(cr, 210, 330, 15, "PAUSE:   \'P\'");
  draw_text(cr, 210, 360, 15, "SPEED UP:  \'space\'");

  return FALSE;
}

// Метод для обновления игры в цикле
gboolean SnakeGameUI::update_game(gpointer user_data) {
  SnakeGameUI* ui = static_cast<SnakeGameUI*>(user_data);
  ui->updateGame();

  return TRUE;
}

#endif
