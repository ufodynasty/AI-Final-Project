#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include<time.h>

void displayBoard(char *board);
int takeTurn(char *board, int player, const char*);
int checkWin(char *board);
int checkFour(char *board, int, int, int, int);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);
int evalBoard(char *board, const char *);
void maximize(int depth, char *board, const char*, int *ret, int alpha);
void minimize(int depth, char *board, const char*, int *ret, int beta);

int main(int argc, char *argv[]){
  srand(time(0));
   const char *PIECES = "XO";
   char board[6 * 7];
   int player1 = 0, player2 = 0, tie = 0;
  for(int r = 0; r < 10000; r++) {
   memset(board, ' ', 6 * 7);

   int turn, done = 0;
   for(turn = 0; turn < 6 * 7 && !done; turn++){
      //displayBoard(board);
      // int result[2];
      // if(turn%2 == 0) {
      //   maximize(9, board, PIECES, result, INT_MAX);
      // } else {
      //   minimize(9, board, PIECES, result, INT_MIN);
      // }
      // printf("Eval: %d\nBest Move: %d\n",result[0], result[1]+1); 
      while(!takeTurn(board, turn % 2, PIECES)){
         //displayBoard(board);   
         //printf("**Column full!**\n");
      }
      done = checkWin(board);
   } 
   //displayBoard(board);

   if(turn == 6 * 7 && !done){
      //printf("It's a tie!\n");
      tie++;
      printf("Player 1: %d\nPlayer 2: %d\nTie: %d\n", player1, player2, tie);
   } else {
      turn--;
      //printf("Player %d (%c) wins!\n", turn % 2 + 1, PIECES[turn % 2]);
      if(turn % 2) {
        player2++;
      } else {
        player1++;
      }
      printf("Player 1: %d\nPlayer 2: %d\nTie: %d\n", player1, player2, tie);
   }
  }
   return 0;

}
void displayBoard(char *board){
   int row, col;
   for(row = 0; row < 6; row++){
      for(col = 0; col < 7; col++){
         printf("%c", board[7 * row + col]);
      }
      printf("\n");
   }
   printf("1234567\n");

}
int takeTurn(char *board, int player, const char *PIECES){
   int row, col = 0;
   //printf("Player %d (%c):\nEnter number coordinate: ", player + 1, PIECES[player]);

  //  while(1){ 
  //     if(1 != scanf("%d", &col) || col < 1 || col > 7 ){
  //        while(getchar() != '\n');
  //        printf("Number out of bounds! Try again.\n");
  //     } else { 
  //        break;
  //     }
  //  }
  //  col--;
  if(player == 0) {
    int result[2];
    maximize(7, board, PIECES, result, INT_MAX);
    col = result[1];
    if(col == -1) {
      col = rand() % 7;
    }

  } else {
    col = rand() % 7;
  }

   for(row = 5; row >= 0; row--){
      if(board[7 * row + col] == ' '){
        board[7 * row + col] = PIECES[player];
        return 1;
      }
   }
   return 0;

}
int checkWin(char *board){
    return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));

}
int checkFour(char *board, int a, int b, int c, int d){
    return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');

}
int horizontalCheck(char *board){
    int row, col;

    for(row = 0; row < 6; row++){
       for(col = 0; col < 7; col++){
          if(checkFour(board, 7 * row + col, 7 * row + ((col + 1) % 7) , 7 * row + ((col + 2) % 7), 7 * row + ((col + 3) % 7))){
            return 1;
          }
       }
    }
    return 0;

}
int verticalCheck(char *board){
    int row, col;

    for(row = 0; row < 3; row++){
       for(col = 0; col < 7; col++){
          if(checkFour(board, 7 * row + col, 7 * row + col + 7, 7 * row + col + 14, 7 * row + col + 21)){
            return 1;
          }
       }
    }
    return 0;

}
int diagonalCheck(char *board){
   int row, col;

   for(row = 0; row < 3; row++){
      for(col = 0; col < 7; col++){
        if(checkFour(board, 7 * row + col, 7 * (row + 1) + ((col + 6) % 7), 7 * (row + 2) + ((col + 5) % 7), 7 * (row + 3) + ((col + 4) % 7)) || checkFour(board, 7 * row + col, 7 * (row + 1) + ((col + 1) % 7), 7 * (row + 2) + ((col + 2) % 7), 7 * (row + 3) + ((col + 3) % 7))){
          return 1;
        }
      }
   }
   return 0;

}

//modified from https://codereview.stackexchange.com/questions/27446/connect-four-game

int evalBoard(char *board, const char *PIECES){
  int row, col, max1 = 0, max2 = 0;
  for(row = 0; row < 6; row++){
    for(col = 0; col < 7; col ++){
      if(board[7 * row + col] != ' ') {
        int player = board[7 * row + col] == PIECES[0] ? 0 : 1;
        int length = 1, max = 0;
        while(board[7 * row + ((col + length) % 7)] == PIECES[player] && length < 7) {
          length ++;
        }
        max = length > max ? length : max;

        length = 1;
        while(row + length < 6 && board[7 * (row + length) + ((col + length) % 7)] == PIECES[player]) {
          length ++;
        }
        max = length > max ? length : max;

        length = 1;
        while(row + length < 6 && board[7 * (row + length) + ((col - length + 7) % 7)] == PIECES[player]) {
          length ++;
        }
        max = length > max ? length : max;

        length = 1;
        while(row + length < 6 && board[7 * (row + length) + col] == PIECES[player]) {
          length ++;
        }
        max = length > max ? length : max;

        if(max > 3) {
          max *= 10;
        }
        if(player == 0) {
          max1 = max > max1 ? max : max1;
        } else {
          max2 = max > max2 ? max : max2;
        }
      }
    }
  }
  return max1-max2;
}

void maximize(int depth, char *board, const char* PIECES, int *ret, int alpha) {
  int max = INT_MIN;
  int move = -1;
  if(depth == 0) {
    ret[0] = evalBoard(board, PIECES);
    ret[1] = move;
  } else if(checkWin(board)){
    ret[0] = max;
    ret[1] = move;
  } else {
    for(int col = 0; col < 7; col++) {
      char* newBoard = strdup(board);
      for(int row = 5; row >= 0; row--){
        if(newBoard[7 * row + col] == ' '){
          newBoard[7 * row + col] = PIECES[0];
          // displayBoard(newBoard);
          // printf("Depth: %d\nCol: %d\n",depth, col);
          int result[2];
          minimize(depth - 1, newBoard, PIECES, result, max);
          // printf("Eval: %d\nMax: %d\n\n", result[0], max);
          move = result[0] > max ? col : move;
          max = result[0] > max ? result[0] : max;
          if(max > alpha) {
            ret[0] = max;
            ret[1] = move;
            return;
          }
          break;
        }
      } 
    }
    if(move == -1) {
      max = evalBoard(board, PIECES);
    }
    ret[0] = max;
    ret[1] = move;
  }
}

void minimize(int depth, char*board, const char* PIECES, int *ret, int beta) {
  int min = INT_MAX;
  int move = -1;
  if(depth == 0) {
    ret[0] = evalBoard(board, PIECES);
    ret[1] = move;
  } else if(checkWin(board)){
    ret[0] = min;
    ret[1] = move;
  } else {
    for(int col = 0; col < 7; col++) {
      char* newBoard = strdup(board);
      for(int row = 5; row >= 0; row--){
        if(newBoard[7 * row + col] == ' '){
          newBoard[7 * row + col] = PIECES[1];
          // displayBoard(newBoard);
          // printf("Depth: %d\nCol: %d\n",depth, col);
          int result[2];
          maximize(depth - 1, newBoard, PIECES, result, min);
          // printf("Eval: %d\nMin: %d\n\n", result[0], min);
          move = result[0] < min ? col : move;
          min = result[0] < min ? result[0] : min;
          if(min < beta) {
            ret[0] = min;
            ret[1] = move;
            return;
          }
          break;
        }
      } 
    }
    if(move == -1) {
      min = evalBoard(board, PIECES);
    }
    ret[0] = min;
    ret[1] = move;
  }
}