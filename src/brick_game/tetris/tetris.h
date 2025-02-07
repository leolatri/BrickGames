#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define M_PI 3.14159265358979323846 /* pi */

#define WIDTH 10
#define HEIGHT 20
#define START_GAME 1
#define CONTINUE_GAME 0
#define GAME_OVER 2
#define PAUSE_GAME 3

// figure--------------------------------------------------------------------------------------------------------------------------
typedef struct {
  int x;
  int y;
} Cub;

typedef struct Figure {
  Cub blocks[4];
  int color;
} Figure;

extern Figure shapes[7];

// action------------------------------------------------------------------------------------------------------------------------------
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Static
} UserAction_t;

typedef struct Game {
  int** field;
  Figure currentFigure;
  UserAction_t action;

  int score;
  int speed;
  int countLine;
  int high_score;
  int level;
} Game;

// dopTablo----------------------------------------------------------------------------------------------------------------------------
typedef struct {
  int** field;
  Figure next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

//-------------------------------------------------------------------------------------------------------------------------------------
void updateCurrentState(Game* tetg, GameInfo_t* tetgInfo);

void drawField(Game* tetg);
void freeField(Game* tetg, GameInfo_t* tetgInfo);
void createField(Game* tetg);
void createFigure(Game* tetg);
int dropFigure(Game* tetg);
void clearFigure(Game* tetg);
void initAction(Game* tetg, int input);
void moveFigure(Game* tetg);
int canMove(Game* tetg);
void rotateFigure(Game* tetg);
void countLine(Game* tetg);
void deleteLine(Game* tetg);
void calcScore(Game* tetg, GameInfo_t* tetgInfo);
int validBlok(Game* tetg, Cub other[4]);

void createInfoField(GameInfo_t* tetg);
void drawInfoField(GameInfo_t* tetg);
void drawNextFigure(GameInfo_t* tetgInfo);
void clearInfFigure(GameInfo_t* tetgInfo);
void printPause(Game* tetg, GameInfo_t* tetgInfo);
void setHighScore(Game* tetg);
void getHighScore(Game* tetg);

#endif