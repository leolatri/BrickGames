#ifndef __Game_snake_CC__
#define __Game_snake_CC__

#include "game.h"

#include "../../gui/desktop/snake_game_ui.h"

using namespace s21;

#include "game.h"

/*Начальное пожение змейки*/
std::vector<Cub> snakeFigure = {{11, 5}, {12, 5}, {13, 5}, {10, 5}};

/*Конструктор, где
  tetg - структура с данными игры;
  tetgInfo - структура с данными игры для информационного поля;

  tetg->action - стратус игры;
  tetg->high_score и tetgInfo->high_score - наибольший счет;
  tetg->level и tetgInfo->level - текущий уровень;
  tetg->score и tetgInfo->score = tetg->score - текущий счет;
  tetg->speed и tetgInfo->speed - текущая скорость;
  tetg->field - игровое поле;
  tetgInfo->field - информационное поле;
  tetg->curSnake - текущее положение змейки;

*/
Game_snake::Game_snake() {
  tetg = new Game();
  tetgInfo = new GameInfo_t();

  tetg->action = Static;
  tetg->high_score = 0;
  tetg->level = 1;
  tetg->score = 0;
  tetg->speed = 150;
  tetg->field = createField();
  tetg->curSnake = snakeFigure;

  tetgInfo->field = createInfoField();
  tetgInfo->high_score = tetg->high_score;
  tetgInfo->score = tetg->score;
  tetgInfo->speed = tetg->level;
  tetgInfo->level = tetg->level;
}

/*Деструктор*/
Game_snake::~Game_snake() { freeField(); }

/*Метод для создания змейки*/
void Game_snake::createSnake() {
  if (tetg) {
    for (int i = 0; i < tetg->curSnake.size() - 1; ++i) {
      int x = tetg->curSnake[i].x;
      int y = tetg->curSnake[i].y;
      if (x < HEIGHT && x >= 0 && y < WIDTH && y >= 0) tetg->field[x][y] = 1;
    }
  }
}

/*Метод, проверяющая валидность движения змейки*/
int Game_snake::valid() {
  int val = 1;
  if (tetg) {
    for (int i = 1; i < tetg->curSnake.size() - 1; ++i) {
      int x = tetg->curSnake[i].x;
      int y = tetg->curSnake[i].y;

      if (tetg->curSnake[0].x < 0 || tetg->curSnake[0].x >= HEIGHT ||
          tetg->curSnake[0].y < 0 || tetg->curSnake[0].y >= WIDTH ||
          (tetg->curSnake[0].x == x && tetg->curSnake[0].y == y))
        val = 0;
    }
  } else
    val = 0;
  return val;
}

/*Метод, отвечающий за движения змейки*/
void Game_snake::moveSnake() {
  if (tetg) {
    if (valid()) {
      int new_x = tetg->curSnake[0].x;
      int new_y = tetg->curSnake[0].y;

      if (tetg->action == Static || tetg->action == Up) {
        --new_x;
      } else if (tetg->action == Left) {
        --new_y;
      } else if (tetg->action == Right) {
        new_y++;
      } else if (tetg->action == Down) {
        ++new_x;
      }

      for (int i = tetg->curSnake.size() - 1; i > 0; --i) {
        tetg->curSnake[i] = tetg->curSnake[i - 1];
      }
      tetg->curSnake[0].x = new_x;
      tetg->curSnake[0].y = new_y;
    } else
      tetg->action = Over;
  }
  eatingApple();
}

/*Метод создания яблока*/
void Game_snake::createApple() {
  if (tetg) {
    bool applePlaced = false;

    while (!applePlaced) {
      tetg->apple.x = rand() % HEIGHT;
      tetg->apple.y = rand() % WIDTH;

      bool occupied = false;
      for (int z = 0; z < tetg->curSnake.size(); ++z) {
        if (tetg->apple.x == tetg->curSnake[z].x &&
            tetg->apple.y == tetg->curSnake[z].y) {
          occupied = true;
          break;
        }
      }
      if (!occupied) {
        tetg->field[tetg->apple.x][tetg->apple.y] = 2;
        applePlaced = true;
      }
    }
  }
}

/*Метод, отвечающий за подсчет очков при поедании змейкой яблока*/
void Game_snake::eatingApple() {
  if (tetg) {
    if (tetg->apple.x == tetg->curSnake[0].x &&
        tetg->apple.y == tetg->curSnake[0].y) {
      tetg->curSnake.push_back(
          {tetg->curSnake[tetg->curSnake.size() - 1].x - 1,
           tetg->curSnake[tetg->curSnake.size() - 1].y});  //?

      ++tetg->score;
      createApple();
    }
  }
}

/*Метод, отвечающий за обновление информации*/
void Game_snake::updateInfo() {
  tetgInfo->score = tetg->score;
  tetgInfo->level = tetg->level;
  tetgInfo->high_score = tetg->high_score;

  int new_level = tetg->score / 5 + 1;
  if (new_level > tetg->level && new_level <= 10) {
    tetg->level = new_level;
    if (tetg->speed > 100) {
      tetg->speed -= 5;
      tetgInfo->speed = tetg->level;
    }
  }
}

/*Метод, отвечающий за отчищение змейкидля последующей отрисовки*/
void Game_snake::clearSnake() {
  if (tetg) {
    for (int i = 0; i < tetg->curSnake.size() - 1; ++i) {
      int x = tetg->curSnake[i].x;
      int y = tetg->curSnake[i].y;
      if (x < HEIGHT && x >= 0 && y < WIDTH && y >= 0) tetg->field[x][y] = 0;
    }
  }
}

/*Метод, отвечающий за создание игрового поля*/
int** Game_snake::createField() {
  tetg->field = (int**)calloc(HEIGHT, sizeof(int*));
  for (int i = 0; i < HEIGHT; ++i) {
    this->tetg->field[i] = (int*)calloc(WIDTH, sizeof(int));
  }

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      this->tetg->field[i][j] = 0;
    }
  }
  tetg->apple = {rand() % HEIGHT, rand() % WIDTH};
  tetg->field[tetg->apple.x][tetg->apple.y] = 2;
  return tetg->field;
}

/*Метод, отвечающий за создание информационного поля*/
int** Game_snake::createInfoField() {
  this->tetgInfo->field = (int**)calloc(HEIGHT, sizeof(int*));
  for (int i = 0; i < HEIGHT; ++i) {
    this->tetgInfo->field[i] = (int*)calloc(20, sizeof(int));
  }

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < 20; ++j) {
      this->tetgInfo->field[i][j] = 0;
    }
  }
  return tetgInfo->field;
}

/*Метод, отвечающий за отчищение игрового и информационного поля*/
void Game_snake::freeField() {
  if (this->tetg) {
    if (this->tetg->field) {
      for (int i = 0; i < HEIGHT; ++i) {
        free(this->tetg->field[i]);
      }
      free(this->tetg->field);
      this->tetg->field = NULL;
    }
  }

  if (this->tetgInfo) {
    if (this->tetgInfo->field) {
      for (int i = 0; i < HEIGHT; ++i) {
        free(this->tetgInfo->field[i]);
      }
      free(this->tetgInfo->field);
      this->tetgInfo->field = NULL;
    }
  }
}

/*Метод, отвечающий за отрисовки игрового поля*/
void Game_snake::drawField(cairo_t* cr) {
  createSnake();
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (tetg->field[i][j] == 0)
        cairo_set_source_rgb(cr, 3.0 / 255.0, 140.0 / 255.0, 44.0 / 255.0);
      else if (tetg->field[i][j] == 1)
        cairo_set_source_rgb(cr, 157.0 / 255.0, 222.0 / 255.0, 242.0 / 255.0);
      else if (tetg->field[i][j] == 2)
        cairo_set_source_rgb(cr, 255.0 / 255.0, 99.0 / 255.0, 71.0 / 255.0);

      cairo_rectangle(cr, j * 20, i * 20, 20, 20);
      cairo_fill(cr);
    }
  }
  clearSnake();
}

/*Метод, отвечающий за отрисовку статуса игры в случае паузы или конца*/
void Game_snake::printPost(cairo_t* cr) {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (tetg->action == Over) {
        draw_text(cr, 65, 190, 20, "GAME");
        draw_text(cr, 60, 212, 20, "O V E R");
        draw_text(cr, 65, 240, 13, "C l i c k \'Q\'");

      } else if (tetg->action == Pause) {
        draw_text(cr, 55, 210, 20, "P A U S E");
      } else if (tetg->action == Win) {
        draw_text(cr, 65, 190, 20, "Y O U");
        draw_text(cr, 40, 213, 20, "HAVE WON");
        draw_text(cr, 65, 240, 13, "C l i c k \'Q\'");
      }
    }
  }
}

/*Метод, отвечающий за отрисовку информациооного поля*/
void Game_snake::drawInfoField(cairo_t* cr) {
  cairo_set_source_rgb(cr, 22.0 / 255.0, 37.0 / 255.0, 28.0 / 255.0);

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 10; j < 20; ++j) {
      cairo_rectangle(cr, j * 20, i * 20, 20, 20);
      cairo_fill(cr);
    }
  }
}

#endif