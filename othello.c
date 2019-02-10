#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLACK_TURN 0
#define WHITE_TURN 1

#define TRUE 1
#define FALSE 0

#define EMPTY_PIECE 46
#define BLACK_PIECE 88
#define WHITE_PIECE 79

void init_board(char board[][8], int *turn, int *skip);
void print_board(const char board[][8], const char placeable[][8]);
void calc_board(char board[][8], char placeable[][8], const int turn);
int get_move(const char placeable[][8], int *is_place, int *skip, int *run);
void place_piece(char board[][8], const int place, int *turn);
void print_winner(const char board[][8]);

/* for debugging */
void print_placeable(char placeable[][8]);

int main(void)
{
	char board[8][8] = { 0, };
	char placeable[8][8] = { 0, };

	int turn, is_place, run, skip;
	int place;

	int i, j;

	/* 처음 보드 초기화 */
	init_board(board, &turn, &skip);

	printf("Welcome to Othello game\n" \
		"%c is for black player " \
		"%c is for white player\n", BLACK_PIECE, WHITE_PIECE);

	printf("Enter column alphabet first, and then enter row number\n" \
		"e.g. A4 or e7\n");
	printf("If you want to resign the game, type gg\n");

	/* 게임이 끝나면 false */
	run = TRUE;
	while (run) {
		calc_board(board, placeable, turn);
		print_board(board, placeable);

		is_place = FALSE;
		while (!is_place) {
			if (turn == BLACK_TURN) {
				printf("BLACK's turn, enter move: ");
			}

			else {
				printf("WHITE's turn, enter move: ");	
			}
			place = get_move(placeable, &is_place, &skip, &run);
		}

		/* 두 플레이어가 모두 놓을 자리가 없는 경우가 게임의 끝 */
		if (skip == 2) { 
			printf("GAME END\n");
			run = 0;
		}
		/* 말을 놓는다 */
		if (place != -1) {
			place_piece(board, place, &turn);
		}
	}

	print_winner(board);

	return 0;
}

void init_board(char board[][8], int *turn, int *skip)
{
	memset(board, EMPTY_PIECE, 64 * sizeof(char));

	board[3][3] = board[4][4] = WHITE_PIECE;
	board[3][4] = board[4][3] = BLACK_PIECE;

	*turn = BLACK_TURN;
	*skip = 0;
	return;
}

void print_board(const char board[][8], const char placeable[][8])
{
	int i, j;


	putchar('\n');
	putchar('\n');

	printf("  a  b  c  d  e  f  g  h\n");

	for (i = 0; i < 8; i++) {
		printf("%d ", i + 1);
		for (j = 0; j < 8; j++) {
			if (placeable[i][j]) {
				printf("*  ");
				continue;
			}
			printf("%c  ", board[i][j]);
		}
		putchar('\n');
	}

	printf("\n\n");
}

void calc_board(char board[][8], char placeable[][8], const int turn)
{
	const int my_piece = turn ? WHITE_PIECE : BLACK_PIECE;
	const int enemy_piece = turn ? BLACK_PIECE : WHITE_PIECE;

	int i, j, k, l;

	int cnt; /* counts the number of enemy piece */

	/* initialize board */
	memset(placeable, 0, 64 * sizeof(char));

	/* 차례가 된 플레이어의 말을 기준으로 계산 */
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j] == my_piece) {
				/* 위쪽 방향 판별 */
				cnt = 0;
				for (k = i - 1, l = j; k >= 0; k--) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
				/* 아래쪽 판별 */
				cnt = 0;
				for (k = i + 1, l = j; k < 8; k++) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
				
				/* 우측 판별 */
				cnt = 0;
				for (k = i, l = j + 1; l < 8; l++) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
				
				/* 좌측 판별 */
				cnt = 0;
				for (k = i, l = j - 1; l >= 0; l--) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
				/* 좌하향 */
				cnt = 0;
				for (k = i + 1, l = j - 1; 
					k < 8 && l >= 0; k++, l--) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
			
				/* 우상향 */
				cnt = 0;
				for (k = i - 1, l = j + 1;
					k >= 0 && l < 8; k--, l++)  {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
		
				/* 우하향 */
				cnt = 0;
				for (k = i + 1, l = j + 1;
					k < 8 && l < 8; k++, l++) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}

				/* 좌상향 */
				cnt = 0;
				for (k = i - 1, l = j - 1;
					k >= 0 && l >= 0; k--, l--) {
					if (board[k][l] == enemy_piece) {
						cnt++;
					}
					else {
						if (board[k][l] == EMPTY_PIECE
						    && cnt)
							placeable[k][l] = 1;
						break;
					}
				}
			}
		}
	}

}

int get_move(const char placeable[][8], int *is_place, int *skip, int *run) {
	char move[3];
	int i, j;
	int sum = 0;
	int ret; /* variable for return */

	/* 놓을 곳이 없으면 패스 */
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			sum += placeable[i][j];
		}
	}

	if (!sum) {
		printf("Cannot place, skip\n");
		*is_place = TRUE;
		(*skip)++;
		return -1;
	}

	/* 입력받음 */
	fgets(move, 3, stdin);
	while (getchar() != '\n')
		;

	if (strcmp(move, "gg") == 0) {
		printf("Resign\n");
		exit(0);
	}

	/* move[0] => column, move[1] => row */
	if (move[0] >= 'A' && move[0] <= 'Z')
		move[0] += 'a' - 'A';

	if (move[0] < 'a' || move[0] > 'z') {
		printf("error: Invalid input\n");
		return -1;

	}
	if (move[1] < '1' || move[1] > '9') {
		printf("error: Invalid input\n");
		return -1;
	}

	ret = ((move[0] - 'a') * 10) + (move[1] - '1');

	if (!placeable[ret % 10][ret / 10]) {
		printf("error: cannot place\n");
		return -1;
	}

	*is_place = TRUE;
	*skip = 0;
	/*  십의자리 가로, 일의자리 세로 */
	return ret;
}

void place_piece(char board[][8], const int place, int *turn)
{
	const int piece = *turn ? WHITE_PIECE : BLACK_PIECE;
	/* enemy's piece */
	const int e_piece = *turn ? BLACK_PIECE : WHITE_PIECE; 
	const int row = place % 10;
	const int column = place / 10;

	/* skip */
	if (place == -1)
		return;

	int i, j;
	int is_flip, cnt;

	/* flip pieces */

	/* 상 */
	is_flip = 0;
	cnt = 0;
	for (i = row - 1, j = column; i >= 0; i--) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}
		
		if (board[i][j] == e_piece)
			cnt++;
		
		if (board[i][j] == EMPTY_PIECE) {
			is_flip = 0;
			break;
		}
	}

	if (is_flip) {
		for (i = row, j = column; board[i][j] != piece; i--)
			board[i][j] = piece;
	}

	/* 하 */
	is_flip = 0;
	cnt = 0;
	for (i = row + 1, j = column; i < 8; i++) {
		if (board[i][j] == piece) {
			/* cnt가 0이면 false 아니면 true */
			is_flip = cnt;
			break;
		}
		
		if (board[i][j] == e_piece)
			cnt++;

		if (board[i][j] == EMPTY_PIECE) {
			is_flip == 0;
			break;
		}
	}

	if (is_flip) {
		for (i = row + 1, j = column; board[i][j] != piece; i++)
			board[i][j] = piece;
	}

	/* 좌 */
	is_flip = 0;
	cnt = 0;
	for (i = row, j = column - 1; j >= 0; j--) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}
		
		if (board[i][j] == e_piece)
			cnt++;

		if (board[i][j] == EMPTY_PIECE) {
			break;
		}
	}

	if (is_flip) {
		for (i = row, j = column - 1; board[i][j] != piece; j--)
			board[i][j] = piece;
	}

	/* 우 */
	is_flip = 0;
	cnt = 0;
	for (i = row, j = column + 1; j < 8; j++) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}
		
		if (board[i][j] == e_piece) {
			cnt++;
		}

		if (board[i][j] == EMPTY_PIECE) {
			break;
		}
	}

	if (is_flip) {
		for (i = row, j = column + 1; board[i][j] != piece; j++)
			board[i][j] = piece;
	}

	/* 우상 */
	is_flip = 0;
	cnt = 0;
	for (i = row - 1, j = column + 1; i >= 0 && j < 8; i--, j++) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}

		if (board[i][j] == e_piece) {
			cnt++;
		}

		if (board[i][j] == EMPTY_PIECE) {
			break;
		}
	}

	if (is_flip) {
		for (i = row - 1, j = column + 1;
			board[i][j] != piece; i--, j++)
			board[i][j] = piece;
	}
	/*  좌하 */
	is_flip = 0;
	cnt = 0;
	for (i = row + 1, j = column - 1; i < 8 && j >= 0; i++, j--) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}

		if (board[i][j] == e_piece) {
			cnt++;
		}

		if (board[i][j] == EMPTY_PIECE) {
			break;
		}
	}
	
	if (is_flip) {
		for (i = row + 1, j = column - 1;
			board[i][j] != piece; i++, j--)
			board[i][j] = piece;
	}
	/* 좌상 */
	is_flip = 0;
	cnt = 0;
	for (i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}

		if (board[i][j] == e_piece) {
			cnt++;
		}

		if (board[i][j] == EMPTY_PIECE) {
			break;
		}
	}
	
	if (is_flip) {
		for (i = row - 1, j = column - 1;
			board[i][j] != piece; i--, j--)
			board[i][j] = piece;
	}

	/* 우하 */
	is_flip = 0;
	cnt = 0;
	for (i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
		if (board[i][j] == piece) {
			is_flip = cnt;
			break;
		}

		if (board[i][j] == e_piece) {
			cnt++;
		}

		if (board[i][j] == EMPTY_PIECE) {
			break;
		}
	}
	
	if (is_flip) {
		for (i = row + 1, j = column + 1;
			board[i][j] != piece; i++, j++)
			board[i][j] = piece;
	}

	board[row][column] = piece;

	*turn = (*turn) ? BLACK_TURN : WHITE_TURN;
}

/* debugging function */
void print_placeable(char placeable[][8])
{
	int i, j;

	printf("  a  b  c  d  e  f  g  h\n");

	for (i = 0; i < 8; i++) {
		printf("%d ", i + 1);
		for (j = 0; j < 8; j++) {
			if (placeable[i][j]) {
				printf("*  ");
			}
			else {
				printf(".  ");
			}
		}
		putchar('\n');
	}

	printf("\n\n");
}

void print_winner(const char board[][8])
{
	int i, j;
	int black = 0, white = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j] == BLACK_PIECE)
				black++;
			if (board[i][j] == WHITE_PIECE)
				white++;
		}
	}

	if (black > white) {
		printf("BLACK WIN!!\n");
	}

	else if (black == white) {
		printf("DRAW!\n");
	}

	else {
		printf("WHITE WIN!!\n");
	}
}
