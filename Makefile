COMPILER = gcc
EXE = game
FILES = main.cpp
LIBS = -I/usr/local/include -isystem. -L. -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++ 
FlAGS =-Wall -Wno-missing-braces -s -O1 

all:
	$(COMPILER) -o $(EXE) $(FILES) $(FlAGS) $(LIBS)
