CC = gcc
OUT = tetris
SRC = game.c tetris.c

$(OUT): $(SRC)
	$(CC) -w $(SRC) -o $(OUT)