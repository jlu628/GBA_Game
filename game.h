#ifndef GAME_H
#define GAME_H

#include "gba.h"
#include "images/bg_game.h"
#include "images/bird.h"
#include "images/game_over.h"
#include "images/loss.h"



                    /* TODO: */

            // Create any necessary structs //


/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
* 
*
*
* Example of a struct to hold state machine data:
* 
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

//Stores the coordination of top left cornor.
typedef struct BirdPos {
  int row;
  int col;
} BirdPos;

typedef struct state {
  int currentState;
  int nextState;
} State;

typedef enum {
  START,
  PLAY,
  PAUSE,
  WIN,
  LOSE,
  END,
} GBAState;

//Row & col defines the coordination of top right cornor of the space letting bird through
//Passed indicates if the bird had already passed the obstacle (i.e. get the point)
typedef struct obstacle {
  int col;
  int row;
  int passed;
} Obstacle;

void moveUp(BirdPos * b);
void moveDown(BirdPos *b);
void moveLeft(BirdPos *b);
void moveRight(BirdPos *b);
void menuMove(int *menuMovingLeft, int *menuMovingUp, BirdPos *b);
void controlMove(BirdPos* b);
void displayReadyMessage(int bottomHalf);
void displayReadyContinue(int bottomHalf, Obstacle *o1, Obstacle *o2, Obstacle *o3, int score, BirdPos* b,  u16* color);
void moveObstacle(Obstacle* o, u16 color, int* score, int* changed);
void drawObstacle(Obstacle* o, u16* color);
void drawScore(int score);
void clearScoreArea(Obstacle* o1, Obstacle* o2, Obstacle* o3, u16* color, BirdPos* b);
void clearBird(BirdPos* b);
void drawPause(void);
void drawContinue(void);
void birdBlink(BirdPos* b, u16* color, Obstacle* o1, Obstacle* o2, Obstacle* o3, int score);
#endif

