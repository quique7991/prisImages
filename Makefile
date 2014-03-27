CC=g++
CFLAGS = -Wall
OPENCV = `pkg-config --cflags --libs opencv` 
OUTPUT = output.o
OBJS = prueba.cpp

all:
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT) $(OPENCV)

clean:
	rm -f *.o