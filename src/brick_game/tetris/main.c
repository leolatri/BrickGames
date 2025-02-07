#include "tetris.h"

int main() {
  srand(time(NULL));
  initscr();
  halfdelay(1);

  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  Game tetg;
  GameInfo_t tetgInfo;

  getHighScore(&tetg);

  tetg.speed = 1500;
  tetg.score = 0;
  tetg.level = 1;
  tetg.countLine = 0;

  tetgInfo.speed = tetgInfo.level;
  tetgInfo.high_score = tetg.high_score;
  tetgInfo.level = tetg.level;
  tetgInfo.score = tetg.score;

  createField(&tetg);
  createInfoField(&tetgInfo);

  int status_of_game = START_GAME;
  int play = START_GAME;
  clock_t start_time = clock();

  tetg.currentFigure = shapes[rand() % 7];
  tetgInfo.next = shapes[rand() % 7];

  while (play) {
    int input = getch();
    initAction(&tetg, input);
    if (tetg.action == Pause || status_of_game == 4) {
      printPause(&tetg, &tetgInfo);
      status_of_game =
          (status_of_game == PAUSE_GAME ? CONTINUE_GAME : PAUSE_GAME);
    }

    if (status_of_game != PAUSE_GAME && tetg.action != Terminate) {
      if (status_of_game == START_GAME) {
        tetg.currentFigure = tetgInfo.next;

        UserAction_t temp_action = tetg.action;
        for (int i = 0; i < WIDTH / 5; ++i) {
          tetg.action = Right;
          moveFigure(&tetg);
        }
        for (int i = 0; i < WIDTH; ++i) {
          tetg.action = (rand() % 2) ? Left : Right;
          moveFigure(&tetg);
        }
        for (int i = 0; i < (rand() % 4); ++i) {
          tetg.action = Up;
          rotateFigure(&tetg);
        }
        tetg.action = temp_action;

        clearInfFigure(&tetgInfo);
        tetgInfo.next = shapes[rand() % 7];
      }

      clearFigure(&tetg);
      moveFigure(&tetg);

      if ((clock() - start_time > tetg.speed) || (tetg.action == Down)) {
        status_of_game = dropFigure(&tetg);
        start_time = clock();
      } else
        status_of_game = CONTINUE_GAME;

      createFigure(&tetg);
      drawField(&tetg);

      if (status_of_game == 1) {
        calcScore(&tetg, &tetgInfo);
        deleteLine(&tetg);
        if (tetg.level >= 9) tetgInfo.level = 10;

        updateCurrentState(&tetg, &tetgInfo);
      }

      drawInfoField(&tetgInfo);
      if (tetgInfo.score > tetg.high_score) {
        tetg.high_score = tetgInfo.score;
        setHighScore(&tetg);
      }

      if (tetg.level >= 10) status_of_game = 4;
    }

    if (tetg.action == Terminate) play = 0;

    refresh();
  }

  freeField(&tetg, &tetgInfo);
  endwin();
  return 0;
}