.SUFFIXES: .cpp

CC = g++
CFLAGS = -g -Wall -std=c++11
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

# SRCS = main.cpp
PROG = img
OBJ = main.o imageProcessing.o

all: $(PROG)

$(PROG):$(OBJ)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ) $(LIBS)
.cpp.o:
	$(CC) $(CFLAGS) -c $?

clean:
	rm -f $(all) $(OBJ)
