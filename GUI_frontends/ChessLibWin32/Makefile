CC              = gcc
DEBUG           = -g -DDEBUG -O1
CFLAGS          = -Wall -std=c11 -I./include
MORE_WARNINGS   = -Wextra -pedantic
RLS             = ChessLib32.exe
DBG             = debug.exe
DWARF2          = -ggdb
LINKER          = -lcomctl32 -lgdi32 -luxtheme
CHESSLIB        = lib/chesslib.c
RELEASE         = -mwindows
WINDOW_SOURCE   = ChessLib32.c
CHLIB_WIN32     = chessboard_win32.c chesspieces_win32.c
RESOURCE        = res\resources.rc
RES_OBJ         = res.o

dbg: compile_resource debug cleanobj

rls: compile_resource release cleanobj

debug: $(WINDOW_SOURCE) $(CHESSLIB) $(CHLIB_WIN32) $(RES_OBJ)
	$(CC) $(CFLAGS) $(DEBUG) -o $(DBG) $^ $(LINKER)

release: $(WINDOW_SOURCE) $(CHESSLIB) $(CHLIB_WIN32) $(RES_OBJ)
	$(CC) $(CFLAGS) $(RELEASE) -o $(RLS) $^ $(LINKER)

compile_resource: $(RESOURCE)
	windres $^ $(RES_OBJ)

.PHONY:
clean: cleanexe cleanobj

cleanexe:
	@del *.exe

cleanobj:
	@del *.o