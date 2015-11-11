 /***********************************************\
 *   Simple ChessLib Win32 Frontend for testing  *
 *               chesspieces_win32.c             *
 *           George Koskeridis (C) 2015          *
 \***********************************************/

#include "ChessLib32.h"


BOOL DRAWN_WORDS;
BOOL DRAWN_PIECES;


VOID InitChessLetters(HWND hwnd)
{
	char *piece_words[] = {"Rook", "Knight", "Bishop", "Queen", "King", "Bishop", "Knight", "Rook"};
	UINT x = (WINDOW_LENGTH/9)/4 - 6, y = (WINDOW_HEIGHT/9) + 31, p_y;
	HDC temp_dc;

	DRAWN_WORDS = TRUE;

	p_y = y - 75;
	for (int i = 0, idx = 0; i < 16; i++, idx++) {
		if (i <= 7) {
			drawnSquares[1][i].piece_handle = CreateWindow("STATIC", 
				"Pawn", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
				x, y, 50, 20, hwnd, NULL, g_hInst, NULL);
			temp_dc = GetDC(drawnSquares[1][i].piece_handle);
			PostMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)temp_dc, (LPARAM)drawnSquares[1][i].piece_handle);
			ReleaseDC(hwnd, temp_dc);
			drawnSquares[1][i].piece_coords.x = x;
			drawnSquares[1][i].piece_coords.y = y;

			drawnSquares[0][i].piece_handle = CreateWindow("STATIC",
				piece_words[idx], WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
				x, p_y, 50, 20, hwnd, NULL, g_hInst, NULL);
			temp_dc = GetDC(drawnSquares[0][i].piece_handle);
			PostMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)temp_dc, (LPARAM)drawnSquares[0][i].piece_handle);
			ReleaseDC(hwnd, temp_dc);
			drawnSquares[0][i].piece_coords.x = x;
			drawnSquares[0][i].piece_coords.y = p_y;
		} else {
			drawnSquares[6][i - 8].piece_handle = CreateWindow("STATIC",
				"Pawn", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
				x, y, 50, 20, hwnd, NULL, g_hInst, NULL);
			temp_dc = GetDC(drawnSquares[6][i - 8].piece_handle);
			PostMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)temp_dc, (LPARAM)drawnSquares[6][i - 8].piece_handle);
			ReleaseDC(hwnd, temp_dc);
			drawnSquares[6][i - 8].piece_coords.x = x;
			drawnSquares[6][i - 8].piece_coords.y = y;

			drawnSquares[7][i - 8].piece_handle = CreateWindow("STATIC",
				piece_words[idx], WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
				x, p_y, 50, 20, hwnd, NULL, g_hInst, NULL);
			temp_dc = GetDC(drawnSquares[7][i - 8].piece_handle);
			PostMessage(hwnd, WM_CTLCOLORSTATIC, (WPARAM)temp_dc, (LPARAM)drawnSquares[7][i - 8].piece_handle);
			ReleaseDC(hwnd, temp_dc);
			drawnSquares[7][i - 8].piece_coords.x = x;
			drawnSquares[7][i - 8].piece_coords.y = p_y;
		}
		if (idx == 7)
			idx = -1;
		x+=72;
		if (i == 7) {
			x = (WINDOW_LENGTH/9)/4 - 6;
			y = 466;
			p_y = y + 75;
		}
	}
}

INT getPieceLetterColor(HWND hwnd)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (hwnd == drawnSquares[i][j].piece_handle) {
				return drawnSquares[i][j].piece_color;
			}
		}
	}
	return EMPTY;
}

VOID drawChessLetters(HWND hwnd)
{
	
}

VOID getChessLetterPos(POINT mouse, POINT *out)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (drawnSquares[i][j].curr_piece != NOPIECE) {
				if (PtInRect(&drawnSquares[i][j].squareArea, mouse)) {
					*out = drawnSquares[i][j].piece_coords;
				}
			}
		}
	}
}
