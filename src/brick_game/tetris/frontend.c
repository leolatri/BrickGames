#include "../../brick_game/tetris/tetris.h"

#ifndef __FRONTEND_C__
#define __FRONTEND_C__

// Game
// Field-------------------------------------------------------------------------------------------------------------------------

void createField(Game* tetg) {
  tetg->field = (int**)calloc(HEIGHT, sizeof(int*));
  for (int i = 0; i < HEIGHT; ++i) {
    tetg->field[i] = (int*)calloc(WIDTH, sizeof(int));
  }

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      tetg->field[i][j] = 0;
    }
  }
}

void initColor() {
  start_color();
  init_pair(0, COLOR_BLUE, COLOR_RED);
  init_pair(1, COLOR_BLUE, 254);
  init_pair(2, COLOR_GREEN, COLOR_GREEN);
  init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(4, COLOR_RED, COLOR_RED);
  init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(6, COLOR_YELLOW, 213);

  init_pair(8, COLOR_YELLOW, 236);
  init_pair(9, COLOR_YELLOW, 260);
}

void drawColoredBlocks(int x, int y, int colorPair) {
  attron(COLOR_PAIR(colorPair));
  mvprintw(x, y * 2, " ");
  mvprintw(x, y * 2 + 1, " ");

  attroff(COLOR_PAIR(colorPair));
}

void drawField(Game* tetg) {
  initColor();

  for (int x = 0; x < HEIGHT; ++x) {
    for (int y = 0; y < WIDTH; ++y) {
      if (tetg->field[x][y] == 0)
        drawColoredBlocks(x, y, 1);
      else if (tetg->field[x][y] == 1 || tetg->field[x][y] == 6)
        drawColoredBlocks(x, y, 2);
      else if (tetg->field[x][y] == 2 || tetg->field[x][y] == 7)
        drawColoredBlocks(x, y, 3);
      else if (tetg->field[x][y] == 3)
        drawColoredBlocks(x, y, 4);
      else if (tetg->field[x][y] == 4)
        drawColoredBlocks(x, y, 5);
      else if (tetg->field[x][y] == 5)
        drawColoredBlocks(x, y, 6);
    }
  }
}

void freeField(Game* tetg, GameInfo_t* tetgInfo) {
  if (tetg) {
    if (tetg->field) {
      for (int i = 0; i < HEIGHT; ++i) {
        free(tetg->field[i]);
      }
      free(tetg->field);
      tetg->field = NULL;
    }
  }

  if (tetgInfo) {
    if (tetgInfo->field) {
      for (int i = 0; i < HEIGHT; ++i) {
        free(tetgInfo->field[i]);
      }
      free(tetgInfo->field);
      tetgInfo->field = NULL;
    }
  }
}

// Figures----------------------------------------------------------------------------------------------------------------------------

void createFigure(Game* tetg) {
  if (tetg) {
    for (int i = 0; i < 4; ++i) {
      int x = tetg->currentFigure.blocks[i].x;
      int y = tetg->currentFigure.blocks[i].y;
      if (x < HEIGHT && x >= 0 && y < WIDTH && y >= 0) {
        tetg->field[x][y] = tetg->currentFigure.color;
      }
    }
  }
}

void clearFigure(Game* tetg) {
  if (tetg) {
    for (int i = 0; i < 4; ++i) {
      int x = tetg->currentFigure.blocks[i].x;
      int y = tetg->currentFigure.blocks[i].y;
      if (x < HEIGHT && x >= 0 && y < WIDTH && y >= 0) {
        tetg->field[x][y] = 0;
      }
    }
  }
}

// Info
// field-------------------------------------------------------------------------------------------------------------------------

void createInfoField(GameInfo_t* tetgInfo) {
  tetgInfo->field = (int**)calloc(HEIGHT, sizeof(int*));
  for (int i = 0; i < HEIGHT; ++i) {
    tetgInfo->field[i] = (int*)calloc(20, sizeof(int));
  }

  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < 20; ++j) {
      tetgInfo->field[i][j] = 0;
    }
  }
}

void drawNextFigure(GameInfo_t* tetgInfo) {
  for (int i = 0; i < 4; ++i) {
    int x = tetgInfo->next.blocks[i].x;
    int y = tetgInfo->next.blocks[i].y;

    tetgInfo->field[x + 5][y + 14] = tetgInfo->next.color;
  }
}

void clearInfFigure(GameInfo_t* tetgInfo) {
  if (tetgInfo) {
    for (int i = 0; i < 4; ++i) {
      int x = tetgInfo->next.blocks[i].x;
      int y = tetgInfo->next.blocks[i].y;
      tetgInfo->field[x + 5][y + 14] = 0;
    }
  }
}

void drawInfoField(GameInfo_t* tetgInfo) {
  initColor();
  drawNextFigure(tetgInfo);

  for (int x = 0; x < HEIGHT; ++x) {
    for (int y = 10; y < 20; ++y) {
      if (tetgInfo->field[x][y] == 0)
        drawColoredBlocks(x, y, 8);
      else if (tetgInfo->field[x][y] == 1 || tetgInfo->field[x][y] == 6)
        drawColoredBlocks(x, y, 2);
      else if (tetgInfo->field[x][y] == 2 || tetgInfo->field[x][y] == 7)
        drawColoredBlocks(x, y, 3);
      else if (tetgInfo->field[x][y] == 3)
        drawColoredBlocks(x, y, 4);
      else if (tetgInfo->field[x][y] == 4)
        drawColoredBlocks(x, y, 5);
      else if (tetgInfo->field[x][y] == 5)
        drawColoredBlocks(x, y, 6);
      else if (tetgInfo->field[x][y] == 9)
        drawColoredBlocks(x, y, 9);
    }
  }

  mvprintw(0, 26, "%s", "GAME INFO");
  mvprintw(7, 21, "%s: %d", "Level", tetgInfo->level);
  mvprintw(8, 21, "%s: %d", "Score", tetgInfo->score);
  mvprintw(9, 21, "%s: %d", "High score", tetgInfo->high_score);
  mvprintw(10, 21, "%s: %d", "Speed", tetgInfo->speed);

  mvprintw(12, 21, "%s", "Left: \'a\'");
  mvprintw(13, 21, "%s", "Right: \'d\'");
  mvprintw(15, 21, "%s", "Down: \'s\'");
  mvprintw(16, 21, "%s", "Rotate: \'w\'");
  mvprintw(18, 21, "%s", "Pause: \'p\'");
  mvprintw(19, 21, "%s", "Terminate: \'q\'");
}
int count_score(int score) {
  int count = 0;
  while (score >= 600 && score > 0) {
    score -= 600;
    ++count;
  }
  return count;
}

void updateCurrentState(Game* tetg, GameInfo_t* tetgInfo) {
  tetgInfo->score = tetg->score;
  tetgInfo->level = tetg->level;
  tetgInfo->high_score = tetg->high_score;
}

// Game over and
// pause----------------------------------------------------------------------------------------------------------------
void printPause(Game* tetg, GameInfo_t* tetgInfo) {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (tetg->action == Pause) {
        tetgInfo->field[i][j] = 9;
        drawColoredBlocks(i, j, 9);
        mvprintw(9, 6, "%s", "P A U S E");
      } else {
        tetgInfo->field[i][j] = 9;
        drawColoredBlocks(i, j, 9);
        mvprintw(8, 7, "%s", "G A M E");
        mvprintw(9, 5, "%s", "O  V  E  R");
        mvprintw(13, 5, "%s", "click \'q\'");
      }
    }
  }
}

// update high
// score-------------------------------------------------------------------------------------------------

void setHighScore(Game* tetg) {
  FILE* file = fopen("high_score_t.txt", "wb");
  fwrite(&tetg->high_score, sizeof(int), 1, file);
  fclose(file);
}

void getHighScore(Game* tetg) {
  FILE* file = fopen("./high_score_t.txt", "rb");
  if (file == NULL) {
    tetg->high_score = 0;
    setHighScore(tetg);
  } else {
    size_t result = fread(&tetg->high_score, sizeof(int), 1, file);
    if (result != 1) {
      tetg->high_score = 0;
    }
    fclose(file);
  }
}

#endif