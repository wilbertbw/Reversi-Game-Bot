#include "reversigamebot.h"

#include <stdio.h>

void printBoard(char board[][26], int n) {
  printf("  ");
  for (int l = 0; l < n; l++) {
    printf("%c", 'a' + l);
  }
  printf("\n");
  for (int i = 0; i < n; i++) {
    printf("%c ", 'a' + i);
    for (int j = 0; j < n; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}

bool positionInBounds(int n, int row, int col) {
  if (row >= n || row < 0) {
    return false;
  }
  if (col >= n || col < 0) {
    return false;
  }
  return true;
}

bool eightDirLegalCheck(char board[][26], int n, int indexRow, int indexCol,
                        char colour) {  // check in 8 directions, prints result
  bool northDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, -1,
                            0);  // north
  bool northeastDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, -1,
                            1);  // northeast
  bool eastDir = checkLegalInDirection(board, n, indexRow, indexCol, colour, 0,
                                       1);  // east
  bool southeastDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, 1,
                            1);  // southeast
  bool southDir = checkLegalInDirection(board, n, indexRow, indexCol, colour, 1,
                                        0);  // south
  bool southwestDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, 1,
                            -1);  // southwest
  bool westDir = checkLegalInDirection(board, n, indexRow, indexCol, colour, 0,
                                       -1);  // west
  bool northwestDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, -1,
                            -1);  // northwest

  if (northDir == true || northeastDir == true || eastDir == true ||
      southeastDir == true || southDir == true || southwestDir == true ||
      westDir == true || northwestDir == true) {
    return true;
  } else {
    return false;
  }
}

int checkAvailableMoves(char board[][26], int n, char colour) {
  for (int indexRow = 0; indexRow < n; indexRow++) {
    for (int indexCol = 0; indexCol < n; indexCol++) {
      if (board[indexRow][indexCol] == 'U') {
        if (eightDirLegalCheck(board, n, indexRow, indexCol, colour) == true) {
          return true;
        }
      }
    }
  }
  return false;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  int subRow, subCol;
  char oppositeColour;

  if (colour == 'W') {
    oppositeColour = 'B';
  } else if (colour == 'B') {
    oppositeColour = 'W';
  }
  // check if the move for "colour" is legal
  if (board[row + deltaRow][col + deltaCol] == oppositeColour &&
      positionInBounds(n, row + deltaRow + deltaRow,
                       col + deltaCol + deltaCol) == true) {
    subRow = row + deltaRow, subCol = col + deltaCol;
    while (board[subRow + deltaRow][subCol + deltaCol] !=
           colour) {  // loop until see white
      if (positionInBounds(n, subRow, subCol) == false) {
        return false;
      } else if (board[subRow + deltaRow][subCol + deltaCol] == 'U') {
        return false;
      }
      subRow += deltaRow;
      subCol += deltaCol;
    }
    return true;
  } else {
    return false;
  }
}

int scoreCount(char board[][26], int n, int indexRow, int indexCol, char colour,
               int deltaRow, int deltaCol) {
  char oppositeColour, currentLetter;
  int countScore = 0;
  if (colour == 'W') {
    oppositeColour = 'B';
  } else if (colour == 'B') {
    oppositeColour = 'W';
  }

  while (board[indexRow][indexCol] != colour &&
         positionInBounds(n, indexRow, indexCol) == true) {
    indexRow += deltaRow;
    indexCol += deltaCol;
    if (positionInBounds(n, indexRow, indexCol) == true &&
        board[indexRow][indexCol] == oppositeColour) {
      countScore++;
    } else if (positionInBounds(n, indexRow, indexCol) == false) {
      return 0;
    }
  }

  return countScore;
}

int highestScoreMove(char board[][26], int n, int indexRow, int indexCol,
                     char colour, int* totalScore) {
  int arr[8] = {0};
  *totalScore = scoreCount(board, n, indexRow, indexCol, colour, -1, 0) +
                scoreCount(board, n, indexRow, indexCol, colour, -1, 1) +
                scoreCount(board, n, indexRow, indexCol, colour, 0, 1) +
                scoreCount(board, n, indexRow, indexCol, colour, 1, 1) +
                scoreCount(board, n, indexRow, indexCol, colour, 1, 0) +
                scoreCount(board, n, indexRow, indexCol, colour, 1, -1) +
                scoreCount(board, n, indexRow, indexCol, colour, 0, -1) +
                scoreCount(board, n, indexRow, indexCol, colour, -1, -1);

  return *totalScore;
}

bool isMoveValid(char board[][26], int n, char colour, char inputRow,
                 char inputCol) {
  int indexRow = inputRow - 'a', indexCol = inputCol - 'a';
  if (positionInBounds(n, indexRow, indexCol) == false) {
    return false;
  }

  return eightDirLegalCheck(board, n, indexRow, indexCol, colour);
}

void loopFlip(char board[][26], int n, int indexRow, int indexCol, char colour,
              int deltaRow, int deltaCol) {
  char oppositeColour;
  if (colour == 'W') {
    oppositeColour = 'B';
  } else if (colour == 'B') {
    oppositeColour = 'W';
  }
  // want to place "colour"
  // loop through, changing opposite colour into "colour"
  do {
    board[indexRow][indexCol] = colour;
    indexRow += deltaRow;
    indexCol += deltaCol;
  } while (board[indexRow][indexCol] != colour &&
           positionInBounds(n, indexRow, indexCol) == true);
}

void flipTiles(char board[][26], int n, char row, char col, char colour) {
  int indexRow = row - 'a';
  int indexCol = col - 'a';
  bool northDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, -1,
                            0);  // north
  bool northeastDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, -1,
                            1);  // northeast
  bool eastDir = checkLegalInDirection(board, n, indexRow, indexCol, colour, 0,
                                       1);  // east
  bool southeastDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, 1,
                            1);  // southeast
  bool southDir = checkLegalInDirection(board, n, indexRow, indexCol, colour, 1,
                                        0);  // south
  bool southwestDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, 1,
                            -1);  // southwest
  bool westDir = checkLegalInDirection(board, n, indexRow, indexCol, colour, 0,
                                       -1);  // west
  bool northwestDir =
      checkLegalInDirection(board, n, indexRow, indexCol, colour, -1,
                            -1);  // northwest

  if (northDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, -1, 0);
  }
  if (northeastDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, -1, 1);
  }
  if (eastDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, 0, 1);
  }
  if (southeastDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, 1, 1);
  }
  if (southDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, 1, 0);
  }
  if (southwestDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, 1, -1);
  }
  if (westDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, 0, -1);
  }
  if (northwestDir == true) {
    loopFlip(board, n, indexRow, indexCol, colour, -1, -1);
  }
}

bool playerMove(char board[][26], int n, char colour, char* userMoveRow,
                char* userMoveCol) {
  printf("Enter move for colour %c (RowCol): ", colour);
  scanf(" %c %c", userMoveRow, userMoveCol);
  bool isValid = isMoveValid(board, n, colour, *userMoveRow, *userMoveCol);

  return isValid;
}

char findOpposite(char turn) {
  if (turn == 'C') {
    turn = 'P';
    return turn;
  } else {
    turn = 'C';
    return turn;
  }
}

int checkValidAndFlip(char board[][26], int row, int col, char colour, int n) {
  // make copy of board, then do the whole flipping process while keeping
  // trackcheckvalidand of tiles flipped
  int totalScore;
  char boardCopy[26][26];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      boardCopy[i][j] = board[i][j];
    }
  }

  return highestScoreMove(boardCopy, n, row, col, colour, &totalScore);
}

void computerMove(char board[][26], int n,
                  char colour) {  // selects first available move
  char computerMoveRow, computerMoveCol, nextRow = 'a', nextCol = 'a';
  int currentScore = -1, nextScore;

  for (int indexRow = 0; indexRow < n; indexRow++) {
    for (int indexCol = 0; indexCol < n; indexCol++) {
      if (board[indexRow][indexCol] == 'U') {
        if (eightDirLegalCheck(board, n, indexRow, indexCol, colour) == true) {
          nextScore = checkValidAndFlip(board, indexRow, indexCol, colour, n);
          if (currentScore < nextScore) {
            currentScore = nextScore;
            computerMoveRow = indexRow + 'a';
            computerMoveCol = indexCol + 'a';
          }
        }
      }
    }
  }
  flipTiles(board, n, computerMoveRow, computerMoveCol, colour);
  printf("Computer places %c at %c%c.\n", colour, computerMoveRow,
         computerMoveCol);
}

char turnToColour(char turn, char playerColour, char computerColour) {
  if (turn == 'P') {
    return playerColour;
  } else {
    return computerColour;
  }
}

void findWinner(char board[][26], int n, char playerColour,
                char computerColour) {
  int playerTiles = 0, computerTiles = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == playerColour) {
        playerTiles++;
      } else if (board[i][j] == computerColour) {
        computerTiles++;
      }
    }
  }
  if (playerTiles > computerTiles) {
    printf("%c player wins.", playerColour);
  } else if (computerTiles > playerTiles) {
    printf("%c player wins.", computerColour);
  }
}

int main(void) {
  // initialize board
  int n;
  char board[26][26], turn;
  printf("Enter the board dimension: ");
  scanf(" %d", &n);

  // ask for computer colour (note: black ALWAYS plays first in this code)
  char computerColour, playerColour;
  printf("Computer plays (B/W): ");
  scanf(" %c", &computerColour);

  if (computerColour == 'B') {
    playerColour = 'W';
    turn = 'C';  // computer turn first
  } else if (computerColour == 'W') {
    playerColour = 'B';
    turn = 'P';  // user turn first
  }

  // fill board
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if ((i == n / 2 - 1 && j == n / 2 - 1) ||
          (i == (n / 2) && j == (n / 2))) {
        board[i][j] = 'W';
      } else if ((i == n / 2 && j == (n / 2) - 1) ||
                 (i == (n / 2) - 1 && j == n / 2)) {
        board[i][j] = 'B';
      } else {
        board[i][j] = 'U';
      }
    }
  }

  // looping play
  char colour, oppositeColour;
  bool gameOver = false, isValid;
  char userMoveRow, userMoveCol;

  printBoard(board, n);

  while (gameOver == false) {
    if (turn == 'C') {
      computerMove(board, n, computerColour);
      printBoard(board, n);
    } else if (turn == 'P') {
      isValid = playerMove(board, n, playerColour, &userMoveRow, &userMoveCol);

      if (isValid == false) {
        printf("Invalid move.\n");
        gameOver = true;
      } else {
        flipTiles(board, n, userMoveRow, userMoveCol, playerColour);
        printBoard(board, n);
      }
    }
    if (gameOver == false) {
      if (turn == 'C') {
        colour = computerColour;
        oppositeColour = playerColour;
      } else if (turn == 'P') {
        colour = playerColour;
        oppositeColour = computerColour;
      }
      if (checkAvailableMoves(board, n, oppositeColour)) {
        turn = findOpposite(turn);
      } else if (checkAvailableMoves(board, n, colour)) {
        printf("%c player has no valid move.\n",
               turnToColour(findOpposite(turn), playerColour, computerColour));
        turn = turn;
      } else {
        gameOver = true;
      }
    }
  }
  findWinner(board, n, playerColour, computerColour);

  return 0;
}