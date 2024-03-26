#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "console.h"

#define BOARD_SIZE 20
#define MOVE_DELAY 10
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int xArr[(BOARD_SIZE - 3) * (BOARD_SIZE - 3)] = {0};
int yArr[(BOARD_SIZE - 3) * (BOARD_SIZE - 3)] = {0};
int x = (BOARD_SIZE - 1) / 2;
int y = (BOARD_SIZE - 1) / 2;

int appleArr[(BOARD_SIZE - 3)][(BOARD_SIZE - 3)] = {0};
int appleX = 2;
int appleY = 2;

char direction = 'r';
int snakeNum = 0;

int score = 0;
int timer = 0;

int done = 0;
int tmp = 0;

void reset() {
  x = (BOARD_SIZE - 1) / 2;
  y = (BOARD_SIZE - 1) / 2;

  appleX = 2;
  appleY = 2;

  direction = 'r';
  snakeNum = 0;

  score = 0;
  timer = 0;

  done = 0;
  tmp = 0;
}

void handleInput() {

  if (snakeNum == 0) {
    if (console::key(console::K_LEFT)) {
      direction = 'l';
    }
    if (console::key(console::K_RIGHT)) {
      direction = 'r';
    }
    if (console::key(console::K_UP)) {
      direction = 'u';
    }
    if (console::key(console::K_DOWN)) {
      direction = 'd';
    }
  }
  else {
    if (console::key(console::K_LEFT) && direction != 'r') {
      direction = 'l';
    }
    if (console::key(console::K_RIGHT) && direction != 'l') {
      direction = 'r';
    }
    if (console::key(console::K_UP) && direction != 'd') {
      direction = 'u';
    }
    if (console::key(console::K_DOWN) && direction != 'u') {
      direction = 'd';
    }
  }
}

void updateSnakeCoord(char direction) {
  if (snakeNum > 0) {
    for (int i = snakeNum; i > 0; i--) {
      xArr[i] = xArr[i - 1];
      yArr[i] = yArr[i - 1];
    }
  }
  
  if (direction == 'l') {
    x--;
  }
  else if (direction == 'r') {
    x++;
  }
  else if (direction == 'u') {
    y--;
  }
  else if (direction == 'd') {
    y++;
  }
  xArr[0] = x;
  yArr[0] = y;
}

void updateAppleCoord() {
    // 뱀이 없는 위치를 저장할 배열 초기화
    int emptySpaces[(BOARD_SIZE - 2) * (BOARD_SIZE - 2)];
    int emptySpaceIndex = 0;

    // 뱀이 없는 위치를 찾아 배열에 저장
    for (int i = 1; i < BOARD_SIZE - 1; i++) {
        for (int j = 1; j < BOARD_SIZE - 1; j++) {
            bool isSnakeBody = false;
            // 뱀의 몸통인지 확인
            for (int k = 0; k < snakeNum; k++) {
                if (xArr[k] == i && yArr[k] == j) {
                    isSnakeBody = true;
                    break;
                }
            }
            // 뱀의 몸통이 아니면 빈 공간 배열에 저장
            if (!isSnakeBody) {
                emptySpaces[emptySpaceIndex] = (i - 1) * (BOARD_SIZE - 2) + j - 1;
                emptySpaceIndex++;
            }
        }
    }

    // 빈 공간 배열이 비어있는지 확인
    if (emptySpaceIndex == 0) {
        // 빈 공간이 없으면 게임 종료
        done = 1; // 이 코드는 게임이 종료되는 조건에 따라서 변경될 수 있습니다.
    } else {
        // 빈 공간 중에서 랜덤하게 선택하여 사과 위치 설정
        int randomIndex = rand() % emptySpaceIndex;
        int selectedEmptySpace = emptySpaces[randomIndex];
        appleX = selectedEmptySpace % (BOARD_SIZE - 2) + 1;
        appleY = selectedEmptySpace / (BOARD_SIZE - 2) + 1;
    }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= console::SCREEN_WIDTH)
    x = console::SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= console::SCREEN_HEIGHT)
    y = console::SCREEN_HEIGHT - 1;
}

void drawBoard() {
  console::draw(0, 0, WALL_LEFT_TOP_STRING);
  console::draw(BOARD_SIZE - 1, 0, WALL_RIGHT_TOP_STRING);
  console::draw(0, BOARD_SIZE - 1, WALL_LEFT_BOTTOM_STRING);
  console::draw(BOARD_SIZE - 1, BOARD_SIZE - 1, WALL_RIGHT_BOTTOM_STRING);

  for (int i = 1; i < BOARD_SIZE - 1; i++) {
      console::draw(i, 0, WALL_HORIZONTAL_STRING);
      console::draw(i, BOARD_SIZE - 1, WALL_HORIZONTAL_STRING);
  }

  for (int i = 1; i < BOARD_SIZE - 1; i++) {
      console::draw(0, i, WALL_VERTICAL_STRING);
      console::draw(BOARD_SIZE - 1, i, WALL_VERTICAL_STRING);
  }

}

void printScore() {
  std::string strScore = std::to_string(score);
  console::draw(BOARD_SIZE / 2 - 4, BOARD_SIZE, "Score: ");
  console::draw(BOARD_SIZE / 2 + 3, BOARD_SIZE, strScore);
}

void getApple() {
  score += 10;
  updateAppleCoord();
  snakeNum++;

  if (direction == 'l') {
    xArr[snakeNum] = xArr[snakeNum - 1] + 1;
    yArr[snakeNum] = yArr[snakeNum - 1];
  }
  else if (direction == 'r') {
    xArr[snakeNum] = xArr[snakeNum - 1] - 1;
    yArr[snakeNum] = yArr[snakeNum - 1];
  }
  else if (direction == 'u') {
    xArr[snakeNum] = xArr[snakeNum - 1];
    yArr[snakeNum] = yArr[snakeNum - 1] + 1;
  }
  else if (direction == 'd') {
    xArr[snakeNum] = xArr[snakeNum - 1];
    yArr[snakeNum] = yArr[snakeNum - 1] - 1;
  }
}

int isDone() {
  if (snakeNum == (BOARD_SIZE - 2) * (BOARD_SIZE - 2) - 1) {
    return 1;
  }
  if (x == 0 || x == BOARD_SIZE - 1 || y == 0 || y == BOARD_SIZE - 1) {
    return 2;
  }
  else {
    for (int i = 1; i <= snakeNum; i++) {
      if (x == xArr[i] && y == yArr[i]) {
        return 2;
      }
    }
  }
  return 0;
}

void game() {

  // 콘솔 라이브러리를 초기화한다.
  console::init();
  //updateAppleCoord();

  while (true) {
    //게임 종료
    if (console::key(console::K_ESC)) {
      exit(0);
    }

    handleInput();

    if (done == 0) {
      if (timer == MOVE_DELAY) {
        timer = 0;
        // 화면을 지운다.
        console::clear();

        // 게임판, 점수 출력
        drawBoard();
        printScore();

        // 뱀 좌표

        updateSnakeCoord(direction);
        restrictInScreen();

        //뱀 출력
        console::draw(x, y, SNAKE_STRING);
        for (int i = 1; i <= snakeNum; i++) {
        console::draw(xArr[i] , yArr[i], SNAKE_BODY_STRING);
        }
        
        //사과 좌표 및 출력
        if (x == appleX && y == appleY) {
          getApple();
        } 
        console::draw(appleX, appleY, APPLE_STRING);

        //게임오버
        done = isDone();
      }
    }
    else if (done == 2) {
      console::draw(BOARD_SIZE / 2 - 5, BOARD_SIZE / 2, "YOU LOSE!");
      console::draw(BOARD_SIZE / 2 - 9, BOARD_SIZE / 2 + 1, "Try again? (Enter)");
      
      if (console::key(console::K_ENTER)) {
        reset();
      }
    }
    else if (done == 1) {
      console::draw(BOARD_SIZE / 2 - 4, BOARD_SIZE / 2, "YOU WIN!");
      console::draw(BOARD_SIZE / 2 - 9, BOARD_SIZE / 2 + 1, "Try again? (Enter)");

      if (console::key(console::K_ENTER)) {
        reset();
      }
    }

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();
    timer++;
  }
}

int main() {
  srand((unsigned)(time(NULL))); 

  game();

  return 0;
}
