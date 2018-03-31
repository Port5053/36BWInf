.PHONY: terminal gui run clean

CFLAGS=-w -std=c++11 -lm -lpthread -O2
CIMG=-L/usr/X11R6/lib -lX11 -O2

terminal: game.o textInterface.h
	g++ -o chess chess.cpp game.o textInterface.h $(CFLAGS)

gui: game.o draw.o
	g++ -o chess chess.cpp game.o draw.o -D GUI $(CFLAGS) $(CIMG)

run: chess
	@ ./chess

clean:
	rm *.o

chess.o: chess.cpp
	g++ -c chess.cpp -D GUI

game.o: game.cpp game.h
	g++ -c game.cpp $(CFLAGS)

draw.o: draw.cpp draw.h
	g++ -c draw.cpp $(CFLAGS)
