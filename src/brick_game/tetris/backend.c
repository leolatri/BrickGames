#include "tetris.h"

#ifndef __BACKEND_C__
#define __BACKEND_C__

Figure shapes[7] = {
    // I
    {{{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}}, 2},

    // O
    {{{0, 0}, {-1, 0}, {0, 1}, {-1, 1}}, 6},

    // T
    {{{-1, 0}, {0, 1}, {-1, 1}, {-2, 1}}, 2},

    // J
    {{{0, 0}, {0, 1}, {-1, 1}, {-2, 1}}, 3},

    // L
    {{{-2, 0}, {0, 1}, {-1, 1}, {-2, 1}}, 4},

    // S
    {{{-1, 0}, {-2, 0}, {0, 1}, {-1, 1}}, 5},

    // Z
    {{{0, 0}, {-1, 0}, {-1, 1}, {-2, 1}}, 6}};

int dropFigure(Game* tetg) {
  int res = 0;
  if (tetg) {
    for (int i = 0; i < 4 && !res; ++i) {
      int currentX = tetg->currentFigure.blocks[i].x;
      int currentY = tetg->currentFigure.blocks[i].y;

      if (currentX + 1 >= HEIGHT) {
        res = 1;
      }

      else if (currentX >= -1 && tetg->field[currentX + 1][currentY] > 0) {
        res = 1;
        for (int i = 0; i < WIDTH; ++i) {
          if (tetg->field[0][i] > 0) res = 4;
        }
      }
    }

    if (res == 0) {
      for (int i = 0; i < 4; ++i) {
        tetg->currentFigure.blocks[i].x += 1;
      }
    }
  }
  return res;
}

void initAction(Game* tetg, int input) {
  switch (input) {
    case 'w':
      tetg->action = Up;
      break;
    case 's':
      tetg->action = Down;
      break;
    case 'a':
      tetg->action = Left;
      break;
    case 'd':
      tetg->action = Right;
      break;
    case 'p':
      tetg->action = Pause;
      break;
    case 'e':
      tetg->action = Start;
      break;
    case 'q':
      tetg->action = Terminate;
      break;
    default:
      tetg->action = Static;
      break;
  }
}

int validBlok(Game* tetg, Cub other[4]) {
  int valid = true;
  for (int i = 0; i < 4; ++i) {
    if (other[i].x >= HEIGHT || other[i].y < 0 || other[i].y >= WIDTH)
      valid = false;
    else if (other[i].x >= 0 && tetg->field[other[i].x][other[i].y] != 0)
      valid = false;
  }
  return valid;
}

void rotateFigure(Game* tetg) {
  int startX = 0, startY = 0;
  int pivotIndex = 0;

  if (tetg->currentFigure.blocks == shapes[3].blocks) {
    pivotIndex = 0;
  } else if (tetg->currentFigure.blocks == shapes[4].blocks) {
    pivotIndex = 2;
  }

  startX = tetg->currentFigure.blocks[pivotIndex].x;
  startY = tetg->currentFigure.blocks[pivotIndex].y;

  Cub newBlocks[4];
  for (int i = 0; i < 4; i++) {
    double x0 = tetg->currentFigure.blocks[i].x - startX;
    double y0 = tetg->currentFigure.blocks[i].y - startY;

    newBlocks[i].x = (int)(-y0 + startX);
    newBlocks[i].y = (int)(x0 + startY);
  }

  if (validBlok(tetg, newBlocks)) {
    for (int i = 0; i < 4; i++) {
      tetg->currentFigure.blocks[i].x = newBlocks[i].x;
      tetg->currentFigure.blocks[i].y = newBlocks[i].y;
    }
  }
}

int canMove(Game* tetg) {
  bool can = true;

  for (int i = 0; i < 4; ++i) {
    int Y = tetg->currentFigure.blocks[i].y;
    int X = tetg->currentFigure.blocks[i].x;

    if (X >= HEIGHT || Y < 0 || Y >= WIDTH) {
      can = false;
    } else {
      if (tetg->action == Left) {
        if (Y - 1 < 0 || (X >= 0 && tetg->field[X][Y - 1] != 0)) {
          can = false;
        }
      } else if (tetg->action == Right) {
        if (Y + 1 >= WIDTH || (X >= 0 && tetg->field[X][Y + 1] != 0)) {
          can = false;
        }
      }
    }
  }
  return can;
}

void moveFigure(Game* tetg) {
  if (tetg) {
    if (canMove(tetg)) {
      for (int i = 0; i < 4; ++i) {
        if (tetg->action == Left) {
          tetg->currentFigure.blocks[i].y -= 1;

        } else if (tetg->action == Right) {
          tetg->currentFigure.blocks[i].y += 1;
        }
      }
    }
    if (tetg->action == Up) {
      rotateFigure(tetg);
    }
  }
}

void countLine(Game* tetg) {
  if (tetg) {
    for (int x = 0; x < HEIGHT; ++x) {
      bool eq = true;
      for (int y = 0; y < WIDTH; ++y) {
        if (tetg->field[x][y] == 0) eq = false;
      }
      if (eq) tetg->countLine++;
    }
  }
}

void deleteLine(Game* tetg) {
  countLine(tetg);
  while (tetg->countLine > 0) {
    for (int x = HEIGHT - 1; x >= 0; --x) {
      bool fullLine = true;
      for (int y = 0; y < WIDTH; ++y) {
        if (tetg->field[x][y] == 0) {
          fullLine = false;
        }
      }
      if (fullLine && dropFigure(tetg) == 1) {
        for (int k = x; k > 0; --k) {
          for (int y = 0; y < WIDTH; ++y) {
            tetg->field[k][y] = tetg->field[k - 1][y];
          }
        }
        for (int y = 0; y < WIDTH; ++y) {
          tetg->field[0][y] = 0;
        }
      }
    }
    tetg->countLine -= 1;
  }
}

void calcScore(Game* tetg, GameInfo_t* tetgInfo) {
  countLine(tetg);
  if (tetg) {
    if (tetg->countLine == 1) {
      tetg->score += 100;
    } else if (tetg->countLine == 2) {
      tetg->score += 300;
    } else if (tetg->countLine == 3) {
      tetg->score += 700;
    } else if (tetg->countLine == 4) {
      tetg->score += 1500;
    }
    int new_level = tetg->score / 600 + 1;  // +1 чтобы начать с уровня 1
    if (new_level > tetg->level && new_level <= 10) {
      tetg->level = new_level;
      if (tetg->speed > 150) {
        tetg->speed -= 150;
        tetgInfo->speed = tetg->level;
      }
    }
  }
}

#endif