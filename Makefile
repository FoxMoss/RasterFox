COMPILER = clang++
EXE = game
FILES = main.cpp camera.cpp
LIBS = -I/usr/local/include -isystem. -L. -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++ -std=c++17
FlAGS =-Wall -Wno-missing-braces -s -O1 

all:
	$(COMPILER) -o $(EXE) $(FILES) $(FlAGS) $(LIBS)
