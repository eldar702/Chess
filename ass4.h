#ifndef __ASS4_H
#define __ASS4_H


void createBoard(char board[][SIZE], char fen[]);
void printBoard(char board[][SIZE]);
int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn);
//for the debugging
//int checkUpperCase(char c);
//void printBoardFromFEN(char fen[]);

#endif
