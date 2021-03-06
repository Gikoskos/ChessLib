CC := gcc
CFLAGS := -Wall -std=gnu11 -fgnu89-inline -O2
LINKER := 
ENABLEDLL := -DBUILD_CHESSLIB_DLL
DEBUG := -g3 -DDEBUG
DLL := chesslib.dll
ELF := chessgame-cli
INC_W_LEVEL := -Wextra -pedantic
BLDFOLDER := build/
CHESSLIB := lib/chesslib.c
AIC := lib/chlib-computer.c
CLILIB := lib/chlib-cli.c
CLISRC := test/chessgame-cli.c
INCLUDE_FOLDER := include/
INC := -I$(INCLUDE_FOLDER)
prefix := /usr

# Build static library archive of ChessLib for Linux to be linked to your program at compile time #
chesslib: $(CHESSLIB) $(AIC)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) -c $(CFLAGS) $(INC) $(NDEBUG) $<; \
	ar -cq libchesslib.a chesslib.o; \
	rm chesslib.o; \
	mv libchesslib.a $(BLDFOLDER)

# Build Linux ELF with debugging flags enabled #
debug: $(CLISRC) $(CHESSLIB) $(AIC) $(CLILIB)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(INC) $(DEBUG) $^ -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build a game that simulates fool's mate
fools: test/rule_tests/foolsmate.c $(CHESSLIB) $(AIC) $(CLILIB)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(INC) $(DEBUG) $^ -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build a game that simulates scholar's mate
scholars: test/rule_tests/scholarsmate.c $(CHESSLIB) $(AIC) $(CLILIB)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(INC) $(DEBUG) $^ -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build a game that simulates the Blackburne-Shilling mate
blackburne: test/rule_tests/blackburnemate.c $(CHESSLIB) $(AIC) $(CLILIB)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(INC) $(DEBUG) $^ -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build tree tests
tree: test/ai_tests/tree.c $(CHESSLIB) $(AIC) $(CLILIB)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(INC) $(DEBUG) $^ -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build DLL #
dll: dllobject dllcompile
dllobject: $(CHESSLIB) $(AIC)
	$(CC) -c $(ENABLEDLL) $(INC) $(CFLAGS) $^
	
dllcompile:
	$(CC) -shared -o $(DLL) chesslib.o chlib-computer.o -Wl,--out-implib,chesslib.a

# Build Windows ELF with the above DLL #
exe: $(CLISRC) $(DLL)
	$(CC) -o chessgame-cli.exe $^

.PHONY: all install cleanall clean cleantxt tests

tests: debug fools scholars tree blackburne

all: chesslib debug fools scholars tree

cleanall: clean cleantxt

clean:
	rm -rf build tests \
	rm *.o \
	rm *.out \
	rm libchesslib.a

cleantxt:
	rm -rf build/*.txt \
	rm *.txt

install: chesslib
	install -m 0755 $(BLDFOLDER)/libchesslib.a $(prefix)/lib
