#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <termios.h>
#include <cmath>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "Matrix_e.h"

using namespace std;

char getch();
/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

pthread_t mainThread;
pthread_mutex_t con_mutex = PTHREAD_MUTEX_INITIALIZER;
char saved_key = 0;
int tty_raw(int fd);	/* put terminal into a raw mode */
int tty_reset(int fd);	/* restore terminal's mode */


void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
  alarm(1);
}

/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

int arrayBlk[] = { 0, 0, 0, 0,
		   1, 1, 1, 1,
		   0, 0, 0, 0,
		   0, 0, 0, 0, -1 };

int iScreenDy = 15;
int iScreenDx = 10;
int iScreenDw = 4;

int* createArrayScreen(int dy, int dx, int dw) {
  int y, x;
  int *array = new int[(dy + dw)*(dx + 2 * dw)];

  for (int y = 0; y < (dy + dw)*(dx + 2 * dw); y++)
    array[y] = 0;

  for (y = 0; y < dy + dw; y++)
    for (x = 0; x < dw; x++)
      array[y * (dx + 2 * dw) + x] = 1;

  for (y = 0; y < dy + dw; y++)
    for (x = dw + dx; x < 2 * dw + dx; x++)
      array[y * (dx + 2 * dw) + x] = 1;

  for (y = dy; y < dy + dw; y++)
    for (x = 0; x < 2 * dw + dx; x++)
      array[y * (dx + 2 * dw) + x] = 1;

  return array;
}

void drawMatrix(Matrix *m) {
  int dy = m->get_dy();
  int dx = m->get_dx();
  int **array = m->get_array();
  for (int y = 0; y < dy - iScreenDw + 1; y++) {
    for (int x = iScreenDw - 1; x < dx - iScreenDw + 1; x++) {
      if (array[y][x] == 0) cout << "□ ";
      else if (array[y][x] == 1) cout << "■ ";
      else cout << array[y][x];//"X ";
    }
    cout << endl;
  }
}

/* Read 1 character - echo defines echo mode */
char getch() {
  char ch;
  int n;
  while (1) {
    pthread_mutex_lock(&con_mutex);
    tty_raw(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    pthread_mutex_unlock(&con_mutex);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
	if (saved_key != 0) {
	  ch = saved_key;
	  saved_key = 0;
	  break;
	}
      }
    }
  }
  return ch;
} // 키값 그대로 내보낸다

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

// 네모블럭 돌리기
int T0D0[] = { 1, 1, 1, 1, -1 };  
int T0D1[] = { 1, 1, 1, 1, -1 };
int T0D2[] = { 1, 1, 1, 1, -1 };
int T0D3[] = { 1, 1, 1, 1, -1 };

//ㅗ 모양 돌리기
int T1D0[] = { 0, 1, 0, 1, 1, 1, 0, 0, 0, -1 };// ㅗ
int T1D1[] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, -1 };// ㅏ
int T1D2[] = { 0, 0, 0, 1, 1, 1, 0, 1, 0, -1 };// ㅜ
int T1D3[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, -1 };// ㅓ

//ㄴ 모양 돌리기(아래가 3칸)
int T2D0[] = { 1, 0, 0, 1, 1, 1, 0, 0, 0, -1 };//ㄴ
int T2D1[] = { 0, 1, 1, 0, 1, 0, 0, 1, 0, -1 };//오른쪽 꺽새
int T2D2[] = { 0, 0, 0, 1, 1, 1, 0, 0, 1, -1 };//ㄱ
int T2D3[] = { 0, 1, 0, 0, 1, 0, 1, 1, 0, -1 };//

//ㄴ 좌우반전 모양 돌리기
int T3D0[] = { 0, 0, 1, 1, 1, 1, 0, 0, 0, -1 };//ㄴ 좌우반전한 모양 
int T3D1[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1, -1 };
int T3D2[] = { 0, 0, 0, 1, 1, 1, 1, 0, 0, -1 };
int T3D3[] = { 1, 1, 0, 0, 1, 0, 0, 1, 0, -1 };//ㄱ(위에 2개)

// 오른쪽 선 번개모양 
int T4D0[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 }; // 오른쪽 선 번개모양
int T4D1[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };
int T4D2[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D3[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };

// 왼쪽 선 번개모양
int T5D0[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };// 왼쪽 선 번개모양
int T5D1[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };
int T5D2[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D3[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };

// ㅡ 
int T6D0[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };// ㅡ
int T6D1[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };// ㅣ
int T6D2[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };// ㅡ
int T6D3[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };// ㅣ

int *setOfBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};
Matrix *setOfObjects[MAX_BLK_TYPES][MAX_BLK_DEGREES];


int main(int argc, char *argv[]) {
  bool newBlockNeeded = false;
  int top = 0;
  int left = iScreenDw + iScreenDx/2 - iScreenDw/2;
  int* arrayScreen = createArrayScreen(iScreenDy, iScreenDx, iScreenDw);
  char key;

  registerAlarm(); // enable a one-second timer
  for(int i = 0; i < MAX_BLK_TYPES; i++)
	  for(int j = 0; j < MAX_BLK_DEGREES; j++)
		  switch(i){
			  case 0:
				  setOfObjects[i][j] = new Matrix(setOfBlockArrays[i * MAX_BLK_TYPES + j],2,2);
				  break;
			  case 6:
				  setOfObjects[i][j] = new Matrix(setOfBlockArrays[i * MAX_BLK_TYPES + j],4,4);
				  break;
			  default:
				  setOfObjects[i][j] = new Matrix(setOfBlockArrays[i * MAX_BLK_TYPES + j],3,3);
				  break;
		  }

  srand(time(NULL)); // 랜덤하게 만들기 위함.

  int t = rand() % MAX_BLK_TYPES;
  int d = 0;
  int col;

  switch (t){
  case 0:
	  col = 2;
	  break;
  case 6:
	  col = 4;
	  break;
  default:
	  col = 3;
	  break;
  }

	
  Matrix *iScreen = new Matrix(arrayScreen, iScreenDy + iScreenDw, iScreenDx + 2 * iScreenDw);
  Matrix *currBlk = setOfObjects[t][d];
  Matrix *tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
  Matrix *tempBlk2 = tempBlk->add(currBlk);
  Matrix *oScreen = new Matrix(iScreen);

  oScreen->paste(tempBlk2, top, left);
  drawMatrix(oScreen); cout << endl;

  while ((key = getch()) != 'q') {
    switch (key) {
    case 'a': left--; break; // move left
    case 'd': left++; break; // move right
    case 's': top++; break; // move down
    case 'w': break; // rotate the block clockwise
    case ' ': break; // drop the block
    default: cout << "unknown key!" << endl;
    }
    delete tempBlk;
    tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    delete tempBlk2;
    tempBlk2 = tempBlk->add(currBlk);
    if (tempBlk2->anyGreaterThan(1)) {
      switch (key) {
      case 'a': left++; break; // undo: move right
      case 'd': left--; break; // undo: move left
      case 's': top--; newBlockNeeded = true; break; // undo: move up
      case 'w': d = (d+1) % MAX_BLK_DEGREES; break; // undo: rotate the block counter-clockwise
      case ' ': break; // undo: move up
      }
      delete tempBlk;
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      delete tempBlk2;
      tempBlk2 = tempBlk->add(currBlk);
    }
    //delete oScreen;    oScreen = new Matrix(iScreen);
    oScreen->paste(iScreen, 0, 0);
    oScreen->paste(tempBlk2, top, left);
    drawMatrix(oScreen); cout << endl;
    if (newBlockNeeded) {
      //delete iScreen;      iScreen = new Matrix(oScreen);
      iScreen->paste(oScreen, 0, 0);
      top = 0; left = iScreenDw + iScreenDx/2 - iScreenDw/2;
      newBlockNeeded = false;
      delete currBlk;
      t = rand() % MAX_BLK_TYPES;
      d = 0;
      switch (t){
        case 0:
            col = 2;
            break;
        case 6:
            col = 4;
            break;
        default:
            col = 3;
            break;
  }
      currBlk = setOfObjects[t][d];
      delete tempBlk;
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      delete tempBlk2;
      tempBlk2 = tempBlk->add(currBlk);

      if (tempBlk2->anyGreaterThan(1)) {
	cout << "Game Over!" << endl;
	exit(0);
      }
      //delete oScreen;      oScreen = new Matrix(iScreen);
      oScreen->paste(iScreen, 0, 0);
      oScreen->paste(tempBlk2, top, left);
      drawMatrix(oScreen); cout << endl;
    }
  }

  exit(0);
}
