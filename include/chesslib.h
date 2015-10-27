/********************************************************************
 *                            chesslib.h                            *
 *                  API for the chesslib.c library                  *
 *                                                                  *
 *               (C)2015 <georgekoskerid@outlook.com>               *
 *                                                                  *
 ********************************************************************/

/*! \file chesslib.h
 *
 * API for the chesslib.c library.
 */

#ifndef CHESS_LIB_H
#define CHESS_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

/// @cond CHESSLIB_DLL
#if defined(__MINGW32__) || defined(_WIN32)
# ifdef BUILD_CHESSLIB_DLL
#  define CHESSLIB_DLL __declspec(dllexport)
# else
#  define CHESSLIB_DLL __declspec(dllimport)
# endif
# include <windows.h>
#endif
/// @endcond


/*! \def WHITE 
 *
 * Random constant value used to represent chess pieces colored white.
 */
#define WHITE 0x1eaf

/*! \def BLACK
 *
 * Random constant value used to represent chess pieces colored black.
 */
#define BLACK 0x1eb0

/*! \def EMPTY 
 *
 * Random constant value used to represent an empty square.
 */
#define EMPTY 0x1eb1


/*! \def PAWN
 *
 * Pawn piece macro used in the 'char current' member of the 'ch_template' structure.
 */
#define PAWN 'P'

/*! \def KING 
 *
 * King piece macro.
 */
#define KING 'K'

/*! \def QUEEN
 *
 * Queen piece macro.
 */
#define QUEEN 'Q'

/*! \def BISHOP 
 *
 * Bishop piece macro.
 */
#define BISHOP 'B'

/*! \def KNIGHT 
 *
 * Knight piece macro.
 */
#define KNIGHT 'N'

/*! \def ROOK 
 *
 * Rook piece macro.
 */
#define ROOK 'R'

/*! \def NOPIECE 
 *
 * Macro used for squares that have no piece on them.
 */
#define NOPIECE 'e'

/// @cond VERSIONING
#define CHESSLIB_MAJOR 0
#define CHESSLIB_MINOR 5
#define CHESSLIB_PATCH 0
#define SSTR(x) STR(x)
#define STR(x) #x
#define CHESSLIB_VERSION_STRING SSTR(CHESSLIB_MAJOR)"."SSTR(CHESSLIB_MINOR)"."SSTR(CHESSLIB_PATCH)
/// @endcond


/*! \def printWhiteMoves()
 * 
 * Small macro used to print the current move list of the White player.
 */
#define printWhiteMoves() printMoves(w_moves)

/*! \def printBlackMoves()
 * 
 * Small macro used to print the current move list of the Black player.
 */
#define printBlackMoves() printMoves(b_moves)


/*! \def printMoves()
 * 
 * Macro used to print all the stored moves on a given MoveNode list.
 */
#define printMoves(x)                                                  \
{                                                                      \
    int move_list_piece_index = 0;                                     \
    char *piece_name[6] = {"Pawns:\n", "King:\n", "Queen:\n",          \
        "Rooks:\n", "Knights:\n", "Bishops:\n"};                       \
    while (move_list_piece_index < 6) {                                \
        fprintf(stdout, "%s", piece_name[move_list_piece_index]);      \
        printMoveList(x[move_list_piece_index++], stdout);             \
    }                                                                  \
}


/*! \struct ch_template
 * 
 * The main chessboard structure used to represent a single square, and the data
 * that is stored on that square, on a chessboard.
 */
struct ch_template {
	char current ;   
	/**< A single character member representing the current chess piece's 
	 * letter: PAWN, KNIGHT, QUEEN, KING, ROOK, BISHOP and NOPIECE are the macros used.*/

	char square[2];
	/**< A two character array for the current square on the board e.g. A1,H4.*/

	bool occ;
	/**< A boolean flag to check if the current square is occupied or not; true
	 * if there's a piece on the current square and false if there's no piece.*/

	int c;
	/**< An integer that stores the piece color; its pre-defined macro values
	 * are BLACK, WHITE and EMPTY.*/
};

/*! \typedef Typedef of struct ch_template to ch_template.
 */
typedef struct ch_template ch_template;



/*! \enum KingState
 * 
 * The KingState enum is used to store the current state of a King on the chessboard.
 * It's used with the WhiteKing and BlackKing KingState enum globals which store the state
 * of both Kings, during a single chess match.
 */
enum KingState {
	check,
	/**< If a King is threatened his global KingState enum gets the check value.*/

	checkmate,
	/**< A player's King gets the checkmate value when that player has no more moves to play.*/

	safe
	/**< Represents the neutral state of a King; when he's neither in check or in checkmate.*/
};

/*! \typedef Typedef of enum KingState to KingState.
 */
typedef enum KingState KingState;



/*! \struct MoveNode
 *
 * MoveNode is the main node structure used to store information for move lists.
 * This structure is exposed due to the fact that the move lists of both players (Black and White)
 * are globals. The person using this library has no use for this structure.
 */
struct MoveNode {
	char start[3];
	/**< String for the square the piece is on; e.g. "A1", "H8"*/

	char end[3];
	/**< String for the square the piece can move to. Same format as start[3].*/

	struct MoveNode *nxt;
	/**< Next node in the list.*/
};

/*! \typedef Typedef of struct MoveNode to MoveNode.
 */
typedef struct MoveNode MoveNode;


/*
 * 
 *round, for both players; each index of the array refers to each piece like so:
 *0 is Pawn (P), 1 is King (K), 2 is Queen (Q), 3 is Rook (R), 4 is Knight (N), 5 is Bishop (B)*/
MoveNode *b_moves[6];
MoveNode *w_moves[6];

/* Global counters for the total moves each player (Black and White) can do on each round.
 * They get a value after every getMoveList() call. */
unsigned white_move_count, black_move_count;


/*! \var BlackKing
 * 
 * Global KingState enum to get the Black King's state after each round.
 * It gets a value after every getMoveList() call.
 */
KingState BlackKing;

/*! \var WhiteKing
 * 
 * Global KingState enum to get the White King's state after each round.
 * It gets a value after every getMoveList() call.
 */
KingState WhiteKing;

/******************************************
 *function prototypes for the main library*
 ******************************************/

int getAllMoves(ch_template chb[][8], int c_flag);

int findOnMoveList(MoveNode *llt, char *tofind);

void initChessboard(ch_template chb[][8]);

void printMoveList(MoveNode *llt, FILE *fd);

void deleteMoveList(MoveNode **llt);

void __attribute__((destructor)) deleteMoves();

bool makeMove(ch_template chb[][8], char *st_move, char *en_move, const int color);

void playMoves(ch_template chb[][8], int *round, unsigned short move_count, ...);

char *getAImove(ch_template chb[][8], const int color, const unsigned short depth);

#ifdef __cplusplus
}
#endif

#endif
