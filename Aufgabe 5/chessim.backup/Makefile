.PHONY: terminal gui run clear

CFLAGS=-Wall -std=c++11 -lm -lpthread -O2
CIMG=-L/usr/X11R6/lib -lX11 -O2

main=chess
game=game.cpp game.h
gui=draw.cpp draw.h
iface=textInterface.h
obj=game.o chess.o
obj_gui=draw.o

terminal: ${obj}
	g++ -o chess ${obj} ${iface} $(CFLAGS)

gui: ${obj}
	g++ -o chess ${obj} ${obj_gui} $(CFLAGS) $(CIMG) -DGUI

run: ${main}
	@ ./${main}

clear:
	rm *.o *.h.gch

${main}.o: ${main}.cpp
	g++ -c ${main}.cpp

game.o: ${game}
	g++ -c ${game} $(CFLAGS)

${obj_gui}: ${gui}
	g++ -c ${gui} $(CFLAGS)
