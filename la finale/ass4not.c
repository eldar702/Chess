#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
// Move logical representation
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol;
	int iSrc, jSrc, iDest, jDest, moveSoldier;
	int isWhite, isCapture, isPromotion, isCheck, isMate, isLegal, isTemp;
} Move;
int isLegal(char board[][SIZE], Move* move, int isWhiteTurn);
int initMove(Move* move);

// PGN characters
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';
const char CAPTURE = 'x';
const char PROMOTION = '=';
const char CHECK = '+';
const char MATE = '#';
const char FIRST_COL = 'a';

// FEN & Board characters
const char WHITE_PAWN = 'P';
const char WHITE_ROOK = 'R';
const char WHITE_KNIGHT = 'N';
const char WHITE_BISHOP = 'B';
const char WHITE_QUEEN = 'Q';
const char WHITE_KING = 'K';
const char BLACK_PAWN = 'p';
const char BLACK_ROOK = 'r';
const char BLACK_KNIGHT = 'n';
const char BLACK_BISHOP = 'b';
const char BLACK_QUEEN = 'q';
const char BLACK_KING = 'k';

// FEN separator for strtok()
const char SEP[] = "/";

// Board characters
const char EMPTY = ' ';
//the function which create the board and save the soldiers in the arrys char board
void createBoard(char board[][SIZE], char fen[]) {
	char i = SIZE -1; //rows
	char j = 0; //columns
	char h = 0; // the variable which run on the string
	char n = 0; // variable for the spaces for numbers
	while (fen[h] != '\0') {
		//if for the letters of the string
		if (((fen[h] >= 'a') && (fen[h] <= 'z')) || ((fen[h] >= 'A') && (fen[h] <= 'Z')))
		{
			board[i][j] = fen[h];
			j++;
		}
		//if for the numbers of the strings
		else if ((fen[h] >= '1') && (fen[h] <= SIZE + 48)) {
			for (n = '0'; n < fen[h]; n++)
			{
				board[i][j] = ' ';
				j++;
			}
		}
		//if for the sleshs of the strings
		else if (fen[h] == '/') {
			i--;
			j = 0;

		}
		h++;
	}
}

//the function which print the 
	void printColumns() {
		char column = toupper(FIRST_COL);
		printf("* |");
		for (int i = 0; i < SIZE; i++) {
			if (i) {
				printf(" ");
			}
			printf("%c", column);
			column++;
		}
		printf("| *\n");
	}
	void printSpacers() {
		printf("* -");
		for (int i = 0; i < SIZE; i++) {
			printf("--");
		}
		printf(" *\n");
	}
	//function which print the board
	void printBoard(char board[][SIZE]) {
		printColumns();
		printSpacers();
		//for loop which print the game board with | and the characters of the soldiers
		for (int i = SIZE - 1; i >= 0; i--)
		{
			printf("%d |", i + 1);
			for (int j = 0; j < SIZE; j++) {
				printf("%c|", board[i][j]);
			}
			printf(" %d\n", i + 1);
		}
			printSpacers();
			printColumns();
	}
		//function which return the rows source
	int findISrc(char pgn[]) {
		int i = 0;
		int iSrc = 0;
		int tempISrc = 0;
		int counter = 0;
		while (pgn[i] != '\0')
		{
			if (pgn[i] >= '1' && pgn[i] <= SIZE + '0') 
			{
				counter++;
				tempISrc = pgn[i] - '0';
			}
			else if (counter == 1)
			{
				iSrc = tempISrc;
			}
		}
		i++;
		if (counter == 2)
		{
			return (iSrc);
		}
		else
			return 0;
	}
		//function which return the columns source
	int findJSrc(char pgn[]) {
			int i = 0;
			int jSrc = 0;
			int tempJSrc = 0;
			int counter = 0;
			while (pgn[i] != '\0')
			{
				if (pgn[i] >= 'a' && pgn[i] <= 'h')
				{
					counter++;
					tempJSrc = pgn[i] - 'a';
				}
			else if (counter == 1)
			{
				jSrc = tempJSrc;
			}
		}
		i++;
		if (counter == 2)
		{
			return (jSrc);
		}
		else
			return 0;
	}
	//function that gives the row destination
	int findIDest(char pgn[]) {
		int i=0;
		int iDest=0;
		while (pgn[i] != '\0')
		{
			if (pgn[i] >= '1' && pgn[i] <= SIZE + '0') {
				iDest = pgn[i] - '0';
			}
			i++;
		}
		iDest = SIZE - iDest;
		return iDest = pgn[i] - '0';
	}
	//function that gives the columns destination
	int FindJDest(char pgn[]) {
		int i = 0;
		int jDest = 0;
		while (pgn[i] != '\0')
		{
			if (pgn[i] >= 'a' && pgn[i] <= 'h') {
				jDest = pgn[i] - 'a';
			}
			i++;
		}
		return (jDest);
	}
	//function that check the character piece fix
	char pieceCase(Move* move, char board[][SIZE], int isWhiteTurn) {
		char piece = move->srcPiece;
		switch (piece) {
		case 'K':
			if (isWhiteTurn)
				return WHITE_KING;
			return BLACK_KING;
			break;
		case 'Q':
			if (isWhiteTurn)
				return WHITE_QUEEN;
			return BLACK_QUEEN;
			break;
		case 'B':
			if (isWhiteTurn)
				return WHITE_BISHOP;
			return BLACK_BISHOP;
			break;
		case 'R':
			if (isWhiteTurn)
				return WHITE_ROOK;
			return BLACK_ROOK;
			break;
		case 'P':
			if (isWhiteTurn)
				return WHITE_PAWN;
			return BLACK_PAWN;
			break;
		case 'N':
			if (isWhiteTurn)
				return WHITE_KNIGHT;
			return BLACK_KNIGHT;
			break;
		default:
			break;
		}
	}
	//function that check the knight moves
	//function that check the diagnosal moves
	int diagnoolMove(Move* move) {
		if (abs(move->iSrc - move->iDest) == abs(move->jSrc - move->jDest))
			return 1;
		return 0;
	}
	//stright move fixed
	int strightMove(Move* move) {
		if (move->iSrc == move->iDest && move->jSrc != move->jDest)
			return 1;
		if (move->iSrc != move->iDest && move->jSrc == move->jDest)
			return 1;
		return 0;
	}
	//check for diagnool if clear
	int isDiagBlock(Move* move, char board[][SIZE]) {
		int dif_i, dif_j;
		if (move->iDest > move->iSrc)// UP
			dif_i = 1;
		else
			dif_i = -1;
		if (move->jDest > move->jSrc) //RIGHT
			dif_j = 1;
		else
			dif_j = -1;

		int i = move->iSrc + dif_i;
		int j = move->jSrc + dif_j;
		for (i,j; i != move->iDest && j != move->jDest; i += dif_i, j += dif_j) {
			char x = board[i][j];
			if (x != EMPTY)
				return 0;
		}
			
		return 1;
	}
	//check for straight if clear
	int isStraightBlock(Move* move, char board[][SIZE]) {
		int dif_i = 0, dif_j = 0;
		if (move->iDest > move->iSrc)
			dif_i = 1;
		if (move->iDest < move->iSrc)
			dif_i = -1;
		if (move->jDest > move->jSrc)
			dif_j = 1;
		if (move->jDest < move->jSrc)
			dif_j = -1;
		/*for (int i = move->iSrc, j = move->jSrc; i != move->iDest && j != move->jDest; i += dif_i, j += dif_j)*/
		for (int i = move->iSrc + dif_i, j = move->jSrc + dif_j; i != move->iDest || j != move->jDest; i += dif_i, j += dif_j)
			if (board[i][j] != EMPTY)
				return 0;
		return 1;
	}
	//function that check the knight move
	int knightMove(Move* move, char board[][SIZE]) {
		int dif_i = abs(move->iDest - move->iSrc);
		int dif_j = abs(move->jDest - move->jSrc);
		if ((dif_i == 1) && (dif_j == 2))
			return 1;
		if ((dif_i == 2) && (dif_j == 1))
			return 1;
		return 0;
	}
	//function that check the rook move
	int rookMove(Move* move, char board[][SIZE]) {
		if (strightMove(move) && isStraightBlock(move, board))
			return 1;
		return 0;
	}
	//function that check the queen move
	int queenMove(Move* move, char board[][SIZE]) {
		if (strightMove(move) && isStraightBlock(move, board))
			return 1;
		if (diagnoolMove(move) && isDiagBlock(move, board))
			return 1;
		return 0;
	}
	//function that check the bishop move
	int bishopMove(Move* move, char board[][SIZE]) {
		if (diagnoolMove(move) && isDiagBlock(move, board))
			return 1;
		return 0;
	}
	//function that check the pawn move
	int pawMove(Move *move, char board[][SIZE], int isWhiteTurn)
	{
		int blackRow = SIZE -2;
		int whiteRow = 1;
		int rowsDifferent = (move->iDest) - (move->iSrc);
		int columnsDifferent = abs(move->jDest - move->jSrc);
		int rows, direction;

		
		if (isWhiteTurn)
		{
			rows = whiteRow;
			//up
			direction = 1;
		}
		else
		{
			rows = blackRow;
			//down
			direction = -1;
		}

		if ((rowsDifferent == 1 * direction) && (columnsDifferent == 0))
		{
			return 1;
		}
		if ((rowsDifferent == 1 * direction) && (move->isCapture && (columnsDifferent == 1)))
		{
			return 1;
		}
		if (((move->iSrc == rows) && (rowsDifferent == 2 * direction)) && (columnsDifferent==0) && isStraightBlock(move, board))
		{
			return 1;
		}
		return 0;
	}
	int kingMove(Move *move)
	{
		
		int columnsDiff = abs(move->jDest - move->jSrc);
		int rowsDiff = abs(move->iDest - move->iSrc);
		//check if the king do a ligal move
		
		if (rowsDiff == 1 && columnsDiff == 1)
		{
			return 1;
		}
		if (rowsDiff == 0 && columnsDiff == 1)
		{
			return 1;
		}
		if (rowsDiff == 1 && columnsDiff == 0)
		{
			return 1;
		}
		return 0;
	}
	int findIsPromotion(int iDest, int isWhiteTurn,char srcPiece, char pgn[])
	{
		int i = 0;
		while (pgn[i] != '\0')
		{
			if (pgn[i] == '=' && (iDest == 0) && (isWhiteTurn))
			{
				return 1;
			}
			else if ((pgn[i] == '=' && (iDest == SIZE - 1) && (!isWhiteTurn)))
			{
				return 1;
			}
			i++;
		}
		return 0;
	}
	//check if i have upper letter
	int checkUpperCase(char c) {
		if ((c >= 'A') && (c <= 'Z')) {
			return 1;
		}
		return 0;
	}
	////check if i have lower letter
	int checkLowerCase(char c) {
		if ((c >= 'a') && (c <= 'z'))
			return 1;
		return 0;
	}
	// struct 0
	int initMove(Move *move) {
		move->srcPiece = 0;
		move->srcRow = 0;
		move->srcCol = 0;
		move->destPiece = 0;
		move->destRow= 0;
		move->destCol = 0;
		move->iSrc = -1;
		move->jSrc = -1;
		move->iDest = -1;
		move->jDest= -1;
		move->moveSoldier = 0;
		move->isWhite = 0;
		move->isCapture= 0;
		move->isPromotion= 0;
		move->isCheck= 0;
		move->isMate= 0;
		move->isLegal= 0;
		move->isTemp = 0;
	}
	// build the struct from string
	//function which find the source of a soldier if i didnt get it from the pgn
	void findSrcPiece(Move* move, char board[][SIZE], int isWhiteTurn) {
		char soldier = pieceCase(move, board, isWhiteTurn);
		int c = move->iSrc;
		int d = move->jSrc;
		if (move->iSrc == -1 && move->jSrc == -1)
		{
			for (int i = SIZE -1; i >= 0; i--)
			{
				for (int j = 0 ; j < SIZE ; j++) {
					if (board[i][j] == soldier)
					{
						move->iSrc = i;
						move->jSrc = j;
						if (isLegal(board, move, isWhiteTurn))
							return;
					}
				}
			}
		}
		else if (move->iSrc == -1 && move->jSrc != -1)
		{
			for (int i = SIZE -1; i >= 0; i--)
			{

				if (board[i][move->jSrc] == soldier)
				{
					move->iSrc = i;
					if (isLegal(board, move, isWhiteTurn))
						return;
				
				}
			}
		}
		if (move->jSrc == -1 && move->iSrc != -1)
		{
			for (int j = 0; j < SIZE; j++)
			{

				if (board[move->iSrc][j] == soldier)
				{
					move->jSrc = j;
					if (isLegal(board, move, isWhiteTurn))
						return;
				}
			}
		}
	}
	//function that complete the nececery details which not always given in pgn
	void completeMove(Move* move, char board[][SIZE], int isWhiteTurn) {
		if (move->iSrc == -1 || move->jSrc == -1)
			//function that find the isrc and jsrc when didnt get from the pgn
			findSrcPiece(move, board, isWhiteTurn);
	}
	void performMove(Move* move, char board[][SIZE], int isWhiteTurn) {
		board[move->iDest][move->jDest] = board[move->iSrc][move->jSrc];
		board[move->iSrc][move->jSrc] = EMPTY;
		if (move->isPromotion) {
			if (isWhiteTurn) {
				board[move->iDest][move->jDest] = move->destPiece;
			}
			else if (!isWhiteTurn) {
				board[move->iDest][move->jDest] = move->destPiece + ('a' - 'A');
			}
		}
	}
		/*	if(isWhiteTurn)
			board[move->iDest][move->jDest] = move->destPiece;
			board[move->iSrc][move->jSrc] = EMPTY;
			if (!isWhiteTurn)
				board[move->iDest][move->jDest] = move->destPiece + ('a' - 'A');
				board[move->iSrc][move->jSrc] = EMPTY;
		}
	}*/

	void scanPgn(char pgn[], Move* move) {
		int i = strlen(pgn)-1;
		int counterNumber = 0;
		int counterLetter = 0;
		if (checkUpperCase(pgn[0]))
			move->srcPiece = pgn[0];
		else
			move->srcPiece = PAWN;
		for (i; i >= 0; i--) {
			//check if there is check in the pgn
			if (pgn[i] == CHECK)
			{
				move->isCheck = 1;
				continue;
			}
			//check if there is MATE in the pgn
			if (pgn[i] == MATE)
			{
				move->isMate = 1;
				continue;
			}
			//check the jdest and jsrc from the pgn
			if (checkLowerCase(pgn[i]) && pgn[i] != 'x') {
				counterLetter++;
				if (counterLetter == 1 && pgn[i] !='x') {
					move->jDest = pgn[i] - 'a';
					continue;
				}
				//check the jsrc from the pgn
				else if (counterLetter == 2 && pgn[i] != 'x') {
					move->jSrc = pgn[i] - 'a';
					continue;
				}
			}
			if (isdigit(pgn[i])) {
				counterNumber++;
				if (counterNumber == 1) {
					move->iDest = pgn[i] - '0' - 1;
					continue;
				}
				//check the Isrc from the pgn
				move->iSrc = pgn[i] - '0' - 1;
				continue;
			}
			//if (move->iSrc == 0 || (move->jSrc == 0)) //////////////////////////////////////////////////
			//check the promotion from the pgn
			if (pgn[i] == PROMOTION) {
				move->isPromotion = 1;
				move->destPiece = pgn[i + 1];
			}
			//check the capture from the pgn
			if (pgn[i] == CAPTURE) {
				move->isCapture = 1;
				continue;
			}
		}
	}
	//function that check if the move is legal in every aspect
	int isLegal(char board[][SIZE], Move* move, int isWhiteTurn)
	{
		//if move is possible
		switch (move->srcPiece)
		{
		case 'K':
			if (!kingMove(move))
			{
				return 0;
			}
			break;
		case 'Q':
			if (!queenMove(move, board))
			{
				return 0;
			}
			break;
		case 'B':
			if (!bishopMove(move, board))
			{
				return 0;
			}
			break;
		case 'R':
			if (!rookMove(move, board))
			{
				return 0;
			}
			break;
		case 'N':
			if (!knightMove(move, board))
			{
				return 0;
			}
			break;
		case 'P':
			if (!pawMove(move, board, isWhiteTurn))
			{
				return 0;
			}
			break;
		default:
			break;
		}
		/*
		The solider didnt move at all
		*/
		if ((move->jSrc == move->jDest) && (move->iSrc == move->iDest))
		{
			return 0;
		}
		//if the piece is "mine" 
		if (isWhiteTurn)
		{
			if (!checkUpperCase(board[move->iSrc][move->jSrc]))
			{
				return 0;
			}
		}
		else
		{
			if (!checkLowerCase(board[move->iSrc][move->jSrc]))
			{
				return 0;
			}
		}
		//if there is mine piece in the dest
		if (isWhiteTurn)
		{
			if (checkUpperCase(board[move->iDest][move->jDest]))
			{
				return 0;
			}
		}
		else
		{
			if (checkLowerCase(board[move->iDest][move->jDest]))
			{
				return 0;
			}
		}
		//eating without sign
		if (!move->isCapture)
		{
			if (isWhiteTurn)
			{
				if (checkLowerCase(board[move->iDest][move->jDest]))
				{
					return 0;
				}
			}
			else
			{
				if (checkUpperCase(board[move->iDest][move->jDest]))
				{
					return 0;
				}
			}

		}
		//capture without any piece there
		if (move->isCapture)
		{
			if (isWhiteTurn)
			{
				if (!checkLowerCase(board[move->iDest][move->jDest]))
					return 0;
			}
			else
			{
				if (!checkUpperCase(board[move->iDest][move->jDest]))
					return 0;
			}
		}
		if (move->isPromotion)
		{
			int endRow = 0;
			if (isWhiteTurn)
			{
				endRow = SIZE - 1;
			}
			if (move->srcPiece != PAWN || move->iDest != endRow)
			{
				return 0;
			}
		}
		if (!move->isPromotion)
		{
			int endRow = 0;
			if (isWhiteTurn)
			{
				if (move->srcPiece == PAWN && move->iDest == endRow)
				{
					return 0;
				}
			}
		}
		//if (!m->checkTool)
		//{
		//	if (!checkCheck(isWhiteTurn, board, m))
		//	{
		//		return 0;
		//	}
		//}
		return 1;
	}
	///////////////////////// is legal not fix
	//int isLegal(char board[][SIZE], Move *move, int isWhiteTurn)
	//{
	//	//check if the move is legal to the selected soldier
	//	switch (move->srcPiece)
	//	{
	//	case 'K':
	//		if (kingMove(move, board) == 0)
	//		{
	//			return 0;
	//		}
	//		break;
	//	case 'Q':
	//		if (queenMove(move, board) == 0)
	//		{
	//			return 0;
	//		}
	//		break;
	//	case 'N':
	//		if (knightMove(move, board) == 0)
	//		{
	//			return 0;
	//		}
	//		break;
	//	case 'B':
	//		if (bishopMove(move, board) == 0)
	//		{
	//			return 0;
	//		}
	//		break;
	//	case 'R':
	//		if (rookMove(move, board) == 0)
	//		{
	//			return 0;
	//		}
	//		break;
	//	case 'P':
	//		if (pawMove(move, board, isWhiteTurn) == 0)
	//		{
	//			return 0;
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//	//if the solider didnt move 
	//	if ((move->jSrc == move->jDest) && (move->iSrc == move->iDest))
	//	{
	//		return 0;
	//	}
	//	//get capture from pgn but there is no soldier there
	//	if (move->isCapture)
	//	{
	//		if (isWhiteTurn)
	//		{
	//			if (!checkLowerCase(board[move->iDest][move->jDest]))
	//				return 0;
	//		}
	//		else
	//		{
	//			if (!checkUpperCase(board[move->iDest][move->jDest]))
	//				return 0;
	//		}
	//	}
	//	//check eating without 'x' sign
	//	if (move->isCapture == 0)
	//	{
	//		if ((isWhiteTurn) && (board[move->iDest][move->jDest]) >= 'a' && ((board[move->iDest][move->jDest]) <= 'z'))
	//			return 0;
	//		else if ((isWhiteTurn == 0) && (board[move->iDest][move->jDest]) >= 'A' && ((board[move->iDest][move->jDest]) <= 'Z'))
	//			return 0;
	//	}
	//	//check if the movment is regard to a soldier from the other colour
	//	//for the white side
	//	if (isWhiteTurn == 1 && board[move->iSrc][move->jSrc] >= 'a' && board[move->iSrc][move->jSrc] >= 'z')
	//	{
	//		return 0;
	//	}
	//	//for the black side
	//	else if (isWhiteTurn == 0 && board[move->iSrc][move->jSrc] >= 'A' && board[move->iSrc][move->jSrc] >= 'Z')
	//	{
	//		return 0;
	//	}

	//	//check if there is a soldier at the same colour in the destination
	//	if (isWhiteTurn)
	//	{
	//		if (((board[move->iDest][move->jDest]) >= 'A') && ((board[move->iDest][move->jDest]) <= 'Z'))
	//		{
	//			return 0;
	//		}
	//		else if (isWhiteTurn == 0)
	//		{
	//			if (((board[move->iDest][move->jDest]) >= 'a') && ((board[move->iDest][move->jDest]) <= 'z'))
	//			{
	//				return 0;
	//			}
	//		}
	//	}
	//	if (move->isPromotion) {
	//		int endRow = 0;
	//		if (isWhiteTurn) {
	//			endRow = SIZE - 1;
	//		}
	//		if (move->srcPiece != 'P' || move->iDest != endRow) {
	//			return 0;
	//		}
	//	}
	//	if (!move->isPromotion) {
	//		int endRow = 0;
	//		if (isWhiteTurn) {
	//			endRow = SIZE - 1;
	//		}
	//		if (move->srcPiece == 'P' && move->iDest == endRow) {
	//			return 0;
	//		}
	//	}

	//	char boardCopy[SIZE][SIZE];
	//	Move temp;
	//	initMove(&temp);
	//	temp.iSrc = move->iDest;
	//	temp.jSrc = move->jDest;
	//	for (int i = 0; i < SIZE; i++)
	//	{
	//		for (int j = 0; j < SIZE; j++)
	//		{
	//			if (board[i][j] = 'K' && !isWhiteTurn)
	//			{
	//				temp.iSrc = move->iDest;
	//				temp.jSrc = move->jDest;
	//			}
	//			if (board[i][j] = 'k' && isWhiteTurn)
	//			{
	//				temp.iSrc = move->iDest;
	//				temp.jSrc = move->jDest;
	//			}
	//		}
	//	}
	//	temp.srcPiece = move->srcPiece;
	//	if (move->isPromotion) {
	//		temp.srcPiece = move->destPiece;
	//	}
	//	temp.isCapture = 1;
	//	temp.isTemp = 1;
	//	for (int i = 0; i < SIZE; i++) {
	//		for (int j = 0; j < SIZE; j++) {
	//			boardCopy[i][j] = board[i][j];
	//		}
	//	}
	//	performMove(move, boardCopy, isWhiteTurn);
	//	if (move->isTemp == 0) {
	//		if (isLegal(boardCopy, &temp, isWhiteTurn) != move->isCheck)
	//			return 0;
	//		
	//	}
	//	return 1;
	//}
	int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn) {
		Move move;
		initMove(&move);
		scanPgn(pgn,&move);
		completeMove(&move, board, isWhiteTurn);
		if (isLegal(board, &move, isWhiteTurn))
		{
			performMove(&move, board, isWhiteTurn);
			return 1;
		}

	}
	
	/*for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				printf("%c", board[i][j]);
			}
			printf("\n");

		}
		*/
	