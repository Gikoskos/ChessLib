 /***********************************************\
 *   Simple ChessLib Win32 Frontend for testing  *
 *                  ChessLib32.c                 *
 *           George Koskeridis (C) 2015          *
 \***********************************************/
 
#include "ChessLib32.h"

#ifdef DEBUG
#define prDSquares()                                                        \
{                                                                           \
    for (int i = 0; i < 8; i++) {                                           \
        for (int j = 0; j < 8; j++) {                                       \
            printf(".squareArea = (left = "                                 \
            "%ld, top = %ld, right = %ld,"                                  \
            " bottom = %ld)\n", drawnSquares[i][j].squareArea.left,         \
            drawnSquares[i][j].squareArea.top,                              \
            drawnSquares[i][j].squareArea.right,                            \
            drawnSquares[i][j].squareArea.bottom);                          \
            printf(".square = '%c%c'\t",                                    \
            drawnSquares[i][j].square[0], drawnSquares[i][j].square[1]);    \
            printf(".curr_piece = %c\n", drawnSquares[i][j].curr_piece);    \
            putchar('\n');                                                  \
    	}                                                                   \
    	printf("--------------\n\n");                                       \
    }                                                                       \
}
#endif


 /**********************
   Global declarations
 **********************/

//Global integers to store the working area's dimension in pixels
static int WORKING_AREA_WIDTH, WORKING_AREA_HEIGHT;

static HWND AboutButton, QuitButton;

static HANDLE CHLIB32_ICON;

static char chlib32_iconpath[] = "icons\\chesslib_icon64.ico";

static ch_template chb32[8][8];

static BOOL GAMEOVER;

 /**********************
   Function prototypes
 **********************/
//Gets the information from the chb32 chessboard
//to the drawnSquares global struct. 
VOID initDrawnSquaresStrct();
VOID InitAboutDlg(HWND hwnd);
VOID gameOver(HWND hwnd);
VOID copyToChb32();
INT_PTR CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


VOID initDrawnSquaresStrct()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			drawnSquares[i][j].piece_color = chb32[i][j].c;
			drawnSquares[i][j].curr_piece  = chb32[i][j].current;
			drawnSquares[i][j].square[0]   = chb32[i][j].square[0];
			drawnSquares[i][j].square[1]   = chb32[i][j].square[1];
		}
	}
}

VOID copyToChb32()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			chb32[i][j].current = drawnSquares[i][j].curr_piece;
			chb32[i][j].c = drawnSquares[i][j].piece_color;
			if (drawnSquares[i][j].piece_color == EMPTY)
				chb32[i][j].occ = false;
			else 
				chb32[i][j].occ = true;
		}
	}
}

INT_PTR CALLBACK AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd, IDOK);
					return (INT_PTR)TRUE;
				default:
					break;
			}
			break;
	}
	return (INT_PTR)FALSE;
}

VOID InitAboutDlg(HWND hwnd)
{
	if (DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTDLG),
					hwnd, AboutDlgProc) == -1) {
		MessageBox(hwnd, "About dialog failed to open!", "Error!", MB_OK | MB_ICONERROR);
	}
}

VOID gameOver(HWND hwnd)
{
	MessageBox(hwnd, (WhiteKing == checkmate)?"Black player wins!":"White player wins!",
	"Game Over", MB_OK | MB_ICONASTERISK);
	GAMEOVER = TRUE;
	//SendMessage(hwnd, WM_DESTROY, (WPARAM)0, (LPARAM)0);
}

LRESULT CALLBACK MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static POINT m_pointer;
	WINDOWPLACEMENT __attribute__((unused)) mainWindowStatus = {.length = sizeof(WINDOWPLACEMENT)};
	static HWND current_piece_clicked;
	static POINT dropped_point, clicked_point;
	static BOOL clicked;
	static HBRUSH pieceLtrsBkgnd = NULL;

	switch (msg) {
		case WM_CREATE:
			SetActiveWindow(hwnd);
			InitChessLetters(hwnd);
			clicked = FALSE;

			AboutButton = CreateWindowEx(0, "BUTTON", "About",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				675, 500, 80, 40, hwnd, (HMENU)IDC_ABOUTBUTTON, 
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
			QuitButton = CreateWindowEx(0, "BUTTON", "Quit",
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				695, 580, 40, 25, hwnd, (HMENU)IDC_QUITBUTTON,
				(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
#if 0
			prDSquares();
#endif
			break;
		case WM_MOUSEMOVE:
			if (!GAMEOVER) {
				m_pointer.x = GET_X_LPARAM(lParam);
				m_pointer.y = GET_Y_LPARAM(lParam);
				if (!clicked) {
					if (BlinkThisSquare(hwnd, m_pointer))
						return PostMessage(hwnd, WM_PAINT, (WPARAM)0, (LPARAM)0);
				} else {
					MoveWindow(current_piece_clicked, m_pointer.x - 25, m_pointer.y - 10, 50, 20, TRUE);
					dropped_point.x = m_pointer.x - 10;
					dropped_point.y = m_pointer.y - 5;
				}
			}
			//GetWindowPlacement(hwnd, &mainWindowStatus);
			//printf("TOP: %ld // LEFT: %ld\n", mainWindowStatus.rcNormalPosition.top, mainWindowStatus.rcNormalPosition.left);
			break;
		case WM_PAINT:
			if ((hdc = BeginPaint(hwnd, &ps))) {
				//DrawChessLetters(hwnd);
				DrawChessboard(hwnd, hdc);
				EndPaint(hwnd, &ps);
			}
			break;
		case WM_CTLCOLORSTATIC:
			{
				int color = getPieceLetterColor((HWND)lParam);

				if (color == BLACK) {
					SetTextColor((HDC)wParam, RGB(200, 200, 200));
					SetBkColor((HDC)wParam, RGB(60, 60, 60));
				} else if (color == WHITE) {
					SetTextColor((HDC)wParam, RGB(0, 25, 51));
					SetBkColor((HDC)wParam, RGB(255, 255, 255));
				}

				if (!pieceLtrsBkgnd)
					pieceLtrsBkgnd = CreateSolidBrush(RGB(100, 100, 120));
				return (INT_PTR)pieceLtrsBkgnd;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_ABOUTBUTTON:
					InitAboutDlg(hwnd);
					break;
				case IDC_QUITBUTTON:
					SendMessage(hwnd, WM_CLOSE, (WPARAM)0, (LPARAM)0);
				default:
					break;
			}
			switch (HIWORD(wParam)) {
				case STN_CLICKED:
					if (!GAMEOVER) {
						if ((HWND)lParam == AboutButton) break;

						if (!clicked) {
							if (ROUND != getPieceLetterColor((HWND)lParam)) break;
							getChessLetterPos(m_pointer, &clicked_point);
							clicked = TRUE;
							resetBlinkingSquares(hwnd);
							current_piece_clicked = (HWND)lParam;
						} else {
							PRECT temp_square = malloc(sizeof(RECT));

							if (!movePiece(clicked_point, dropped_point, temp_square)) {
								MoveWindow(current_piece_clicked, clicked_point.x, clicked_point.y, 50, 20, TRUE);
							} else {
								temp_square->right = temp_square->right - 60;
								temp_square->bottom = temp_square->bottom - 45;
								MoveWindow(current_piece_clicked, temp_square->right, temp_square->bottom, 50, 20, TRUE);
								copyToChb32();
								ROUND = (ROUND == WHITE)?BLACK:WHITE;
								deleteMoves();
								getAllMoves(chb32, ROUND);
								if (BlackKing == checkmate || WhiteKing == checkmate)
									gameOver(hwnd);
							}
							free(temp_square);

							current_piece_clicked = NULL;
							clicked = FALSE;
						}
					}
					break;
				case STN_DISABLE:
					ShowWindow((HWND)lParam, SW_HIDE);
				default:
					break;
			}
			break;
		case WM_CLOSE:
#ifndef DEBUG
			if (MessageBox(hwnd, "Are you sure you want to leave?", 
					"Quiting...", MB_ICONASTERISK | MB_YESNO) == IDNO)
				break;
#endif
		case WM_DESTROY:
			DeleteObject((HGDIOBJ)pieceLtrsBkgnd);
			DestroyWindow(hwnd);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	BOOL bRet = TRUE;
	RECT wrkspace_px;
	
	CHLIB32_ICON = LoadImage(GetModuleHandle(NULL), chlib32_iconpath, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (!CHLIB32_ICON) {
		MessageBox(NULL, "Failed opening ""icons\\chesslib_icon64.ico"" icon!", "Error!",	
			MB_ICONERROR | MB_OK);
		return -1;
	}

	//Get the working desktop area (width x height) in pixels.
	//The taskbar is not included in this area, so it's not always
	//the resolution of the monitor that shows the program.
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&wrkspace_px, SPIF_SENDCHANGE);
	WORKING_AREA_WIDTH = wrkspace_px.right;
	WORKING_AREA_HEIGHT = wrkspace_px.bottom;

	g_hInst = hInstance;
	GAMEOVER = FALSE;

	//Initialisations for ChessLib
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			drawnSquares[i][j].piece_handle = NULL;
	
	initChessboard(chb32);
	ROUND = WHITE;
	getAllMoves(chb32, ROUND);
	initDrawnSquaresStrct();

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.lpfnWndProc = MainWindowProcedure;
	wc.hInstance = hInstance;
	wc.hIconSm = wc.hIcon = CHLIB32_ICON;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_DESKTOP);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "ChessLib_Win32";

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Main Window class registration Failed!", "Error!",
			MB_ICONERROR | MB_OK);
		return -1;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "ChessLib_Win32", "ChessLib Win32",
	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	CW_USEDEFAULT, CW_USEDEFAULT, 800, 665, NULL, NULL, hInstance, NULL);

	if (!hwnd) {
		MessageBox(NULL, "Window creation failed!", "Error!", MB_ICONERROR | MB_OK);
		return -2;
	}

	InitCommonControls();
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (bRet) {
		bRet = GetMessage(&Msg, NULL, 0, 0);
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
