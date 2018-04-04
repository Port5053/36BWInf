.PHONY: foo image gui

COMPILE=g++ wall.cpp -w -std=c++11 -O3
CIMG=-L/usr/X11R6/lib -lX11 -lm -lpthread

foo: wall.cpp
	$(COMPILE) -o wall

image: wall.cpp
	$(COMPILE) $(CIMG) -DIMAGE -o walliamge

gui: wall.cpp
	$(COMPILE) $(CIMG) -DIMAGE -DGUI -o wallgui
