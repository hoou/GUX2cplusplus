NAME = piskvorky
CC = g++
CFLAGS = -static-libstdc++ `pkg-config --cflags gtk+-3.0` -std=c++11 -pedantic -Wall
LIBS = `pkg-config --libs gtk+-3.0`

all:
	$(CC) $(CFLAGS) Application.cpp Cell.cpp GameLogic.cpp Gui.cpp Player.cpp utils.h main.cpp -o $(NAME) $(LIBS)
 
clean:
	rm -f $(NAME)
