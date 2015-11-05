 /***********************************************\
 *   Simple ChessLib Win32 Frontend for testing  *
 *                  ChessLib32.h                 *
 *           George Koskeridis (C) 2015          *
 \***********************************************/
 
#pragma once

#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include <uxtheme.h>
#include <wchar.h>
#include <chesslib.h>

#define WINDOW_LENGTH 650
#define WINDOW_HEIGHT 665

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

#define IDC_ABOUTBUTTON 0xfade
#define IDC_QUITBUTTON 0xdeaf
#define IDD_ABOUTDLG   101


typedef struct drawnSquareData {
	RECT squareArea;
	char square[2];
	char curr_piece;
	int piece_color;
	int square_color;
	POINT piece_coords;
	HWND piece_handle;
	BOOL blink;
} drawnSquareData;

drawnSquareData drawnSquares[8][8];


HINSTANCE g_hInst;

int ROUND;



//Draws a small and simple chessboard on a device context and
//fills all the drawnSquare[n].squareArea members with the pixel
//coordinates of every square drawn in the board (this happens
//only the first time this function is called, to improve performance).
VOID DrawChessboard(_In_ HWND hwnd, _In_ HDC chessboard_dc);

//Draws chess piece words, in a board drawn with the DrawChessboard
//function, just like they are positioned in a chess board in the
//beginning of a traditional chess game. Also initializes the values
//of the chess piece words' window handle arrays for each player.
//Should be called at least once in the beginning of the game.
VOID InitChessLetters(_In_ HWND hwnd);

VOID resetBlinkingSquares(_In_ HWND hwnd);

VOID getChessLetterPos(_In_ POINT mouse, _Out_ POINT *out);

VOID drawChessLetters(_In_ HWND hwnd);

BOOL movePiece(_In_ POINT picked, _In_ POINT dropped, _Out_ PRECT square);

INT getPieceLetterColor(_In_ HWND hwnd);

BOOL BlinkThisSquare(_In_ HWND hwnd, _In_ POINT mouse);
