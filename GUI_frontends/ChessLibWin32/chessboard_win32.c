 /***********************************************\
 *   Simple ChessLib Win32 Frontend for testing  *
 *               chessboard_win32.c              *
 *           George Koskeridis (C) 2015          *
 \***********************************************/

#include "ChessLib32.h"

bool _isOnList(const char *start_move, const char *end_move, const char piece, const int color);


VOID resetBlinkingSquares(HWND hwnd)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (drawnSquares[i][j].blink) {
				drawnSquares[i][j].blink = FALSE;
				InvalidateRect(hwnd, &drawnSquares[i][j].squareArea, FALSE);
			}
		}
	}
}

BOOL BlinkThisSquare(HWND hwnd, POINT mouse)
{
	if (!mouse.x && !mouse.y) return FALSE;

	BOOL retvalue = FALSE;
	int i, j, sel_piece;
	char sel_square[2] = {'n', 'o'};

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (PtInRect(&drawnSquares[i][j].squareArea, mouse)) {
				if (!drawnSquares[i][j].blink) {
					InvalidateRect(hwnd, &drawnSquares[i][j].squareArea, FALSE);
					drawnSquares[i][j].blink = TRUE;
					retvalue = TRUE;
				}
				sel_piece = drawnSquares[i][j].curr_piece;
				sel_square[0] = drawnSquares[i][j].square[0];
				sel_square[1] = drawnSquares[i][j].square[1];
			} else {
				if (drawnSquares[i][j].blink)
					InvalidateRect(hwnd, &drawnSquares[i][j].squareArea, FALSE);
				drawnSquares[i][j].blink = FALSE;
			}
		}
	}
	if (sel_square[0] == 'n') return retvalue;

	MoveNode *curr;
	int p;
	switch (sel_piece) {
		case PAWN:
			p = 0;
			break;
		case KING:
			p = 1;
			break;
		case QUEEN:
			p = 2;
			break;
		case ROOK:
			p = 3;
			break;
		case KNIGHT:
			p = 4;
			break;
		case BISHOP:
			p = 5;
			break;
		default:
			return retvalue;
	}
	if (ROUND == WHITE) {
		curr = w_moves[p];
	} else {
		curr = b_moves[p];
	}
	if (!curr)
		return retvalue;
	while (curr) {
		if (sel_square[0] == curr->start[0] && sel_square[1] == curr->start[1]) {
			unsigned short y = '8' - curr->end[1], x = curr->end[0] - 'A';
			InvalidateRect(hwnd, &drawnSquares[y][x].squareArea, FALSE);
			drawnSquares[y][x].blink = TRUE;
		}
		curr = curr->nxt;
	}

	return retvalue;
}

VOID DrawChessboard(HWND hwnd, HDC chessboard_dc)
{
	//the color of the black squares
	HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0)), white_brush = CreateSolidBrush(RGB(255, 255, 255));
	static BOOL chessboard_is_drawn;

	if (!chessboard_is_drawn) {
		//temporary values to store current square pixel data
		PRECT black_square = malloc(sizeof(RECT)), white_square = malloc(sizeof(RECT));
		//div_offset determines the size of the board that will be drawn.
		//This makes the board sort of modular; the only downside is that the
		//board letters and numbers are fixed so they don't change their position
		//based on this value.
		unsigned short div_offset = 9;

		black_square->top = 0;
		black_square->right = (WINDOW_LENGTH/div_offset)*2;
		black_square->bottom = WINDOW_HEIGHT/div_offset;
		black_square->left = WINDOW_LENGTH/div_offset;
		for (int i = 1, x = 0, y = 0, white_idx = 0, black_idx = 0; i <= 32; i++) {
			white_square->top = black_square->top;
			white_square->bottom = black_square->bottom;
			if (i <= 4 || (i > 8 && i <= 12) || (i > 16 && i <= 20) || (i > 24 && i <= 28)) {
				white_square->left = black_square->left - (WINDOW_LENGTH/div_offset);
				white_square->right = black_square->left;
			} else {
				white_square->left = black_square->right;
				white_square->right = black_square->right + (WINDOW_LENGTH/div_offset);
			}
#if 0
				printf("WHITE SQUARE: Left Top (X = %ld, Y = %ld) Right Bottom (X = %ld, Y = %ld)\n", 
				white_square->left, white_square->top, white_square->right, white_square->bottom);
				printf("BLACK SQUARE: Left Top (X = %ld, Y = %ld) Right Bottom (X = %ld, Y = %ld)\n", 
				black_square->left, black_square->top, black_square->right, black_square->bottom);
#endif
			if (i == 1) {
				white_idx = x;
				black_idx = x + 1;
			} else if (!x) {
				white_idx = (white_idx == 7)?x:(x+1);
				black_idx = (black_idx == 7)?x:(x+1);
			} else {
				if (white_idx == (x - 2)) {
					white_idx = x;
					black_idx = x + 1;
				} else {
					white_idx = x + 1;
					black_idx = x;
				}
			}
			CopyRect(&drawnSquares[y][white_idx].squareArea, white_square);
			CopyRect(&drawnSquares[y][black_idx].squareArea, black_square);
			drawnSquares[y][white_idx].square_color = WHITE;
			drawnSquares[y][black_idx].square_color = BLACK;
			drawnSquares[y][white_idx].blink = FALSE;
			drawnSquares[y][black_idx].blink = FALSE;
			x+=2;
			if (!(i%4)) {
				x = 0;
				y++;
			}

			FillRect(chessboard_dc, black_square, black_brush);
			FillRect(chessboard_dc, white_square, white_brush);
			black_square->right += (WINDOW_LENGTH/div_offset)*2;
			black_square->left += (WINDOW_LENGTH/div_offset)*2;

			if (!(i%4)) {
				if (!(i%8)) {
					black_square->left = WINDOW_LENGTH/div_offset;
					black_square->right = (WINDOW_LENGTH/div_offset)*2;
				} else {
					black_square->left = 0;
					black_square->right = WINDOW_LENGTH/div_offset;				
				}
				black_square->top += WINDOW_HEIGHT/div_offset;
				black_square->bottom += WINDOW_HEIGHT/div_offset;
			}
		}
		free(white_square);
		free(black_square);
		chessboard_is_drawn = TRUE;
	} else {
		//CreateHatchBrush(HS_CROSS, RGB(150, 25, 20));
		HBRUSH white_blinking_square = CreateSolidBrush(RGB(102, 178, 255));
		HBRUSH black_blinking_square = CreateSolidBrush(RGB(90, 10, 200));
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				SetBkMode(chessboard_dc, TRANSPARENT);
				if (drawnSquares[i][j].blink) {
					FillRect(chessboard_dc, &drawnSquares[i][j].squareArea, 
					(ROUND == WHITE)?white_blinking_square:black_blinking_square);
				} else {
					FillRect(chessboard_dc, &drawnSquares[i][j].squareArea,
					(drawnSquares[i][j].square_color == BLACK)?black_brush:white_brush);
				}
			}
		}
		DeleteObject((HGDIOBJ)white_blinking_square);
		DeleteObject((HGDIOBJ)black_blinking_square);
	}
	DeleteObject((HGDIOBJ)black_brush);
	DeleteObject((HGDIOBJ)white_brush);

	// Draw the numbers and letters for the chessboard
	char s[2];
	unsigned short offs_x = 620, offs_y = 65;
	SetBkMode(chessboard_dc, TRANSPARENT);

	s[0] = 'A';
	s[1] = '\0';
	
	for (char c = 'A'; c <= 'H'; offs_x -= 72, c++, s[0] = c)
		TextOut(chessboard_dc, WINDOW_LENGTH - offs_x, WINDOW_HEIGHT - offs_y, s, 1);

	s[0] = '1';
	offs_y += 63;
	for (int i = 1; i <= 8; i++, offs_y += 73, s[0] = i + '0')
		TextOut(chessboard_dc, WINDOW_LENGTH - offs_x, WINDOW_HEIGHT - offs_y, s, 1);
	MoveToEx(chessboard_dc, 640, 0, NULL);
	LineTo(chessboard_dc, 640, 665);
}

BOOL movePiece(POINT picked, POINT dropped, PRECT square)
{
	BOOL retvalue = FALSE;

	if (!square) return retvalue;

	char piece = NOPIECE;
	int color = EMPTY, st_i, st_j, en_i, en_j;
	char temp_st[] = "NU", temp_en[] = "NU";
	st_i = st_j = en_i = en_j = -1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (PtInRect(&drawnSquares[i][j].squareArea, picked)) {
				st_i = i;
				st_j = j;
				temp_st[0] = drawnSquares[i][j].square[0];
				temp_st[1] = drawnSquares[i][j].square[1];
				piece = drawnSquares[i][j].curr_piece;
				color = drawnSquares[i][j].piece_color;
			}
			if (PtInRect(&drawnSquares[i][j].squareArea, dropped)) {
				en_i = i;
				en_j = j;
				temp_en[0] = drawnSquares[i][j].square[0];
				temp_en[1] = drawnSquares[i][j].square[1];
			}
		}
	}
	if (temp_st[0] == 'N' || temp_en[0] == 'N' || 
		st_i == -1 || st_j == -1 || en_i == -1 || en_j == -1) return retvalue;

	if (ROUND != color) return retvalue;

	if (_isOnList(temp_st, temp_en, piece, color)) {
		if (drawnSquares[en_i][en_j].curr_piece != NOPIECE) {
			//MoveWindow(drawnSquares[en_i][en_j].piece_handle, 630, 30, 50, 20, TRUE);
			EnableWindow(drawnSquares[en_i][en_j].piece_handle, FALSE);
		}
		drawnSquares[en_i][en_j].curr_piece       = piece;
		drawnSquares[en_i][en_j].piece_handle     = drawnSquares[st_i][st_j].piece_handle;
		drawnSquares[en_i][en_j].piece_color      = color;
		drawnSquares[en_i][en_j].piece_coords.x   = dropped.x;
		drawnSquares[en_i][en_j].piece_coords.y   = dropped.y;
		drawnSquares[st_i][st_j].curr_piece       = NOPIECE;
		drawnSquares[st_i][st_j].piece_color      = EMPTY;
		drawnSquares[st_i][st_j].piece_handle     = NULL;
		drawnSquares[st_i][st_j].piece_coords.x   = 0;
		drawnSquares[st_i][st_j].piece_coords.y   = 0;
		CopyRect(square, &drawnSquares[en_i][en_j].squareArea);
		retvalue = TRUE;
	}

	return retvalue;
}
