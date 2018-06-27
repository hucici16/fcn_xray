.SUFFIXES: .cpp

CC = g++
CFLAGS = -g -Wall -std=c++11
OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

# SRCS = main.cpp
PROG = allClasses
PROG2 = singleClass
PROG3 = singleClassCropped
OBJ = allClasses.o imageProcessing.o
OBJ2 = singleClass.o imageProcessing.o
OBJ3 = singleClassCropped.o imageProcessing.o

all: $(PROG) $(PROG2) $(PROG3)

$(PROG):$(OBJ)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ) $(LIBS)
$(PROG2):$(OBJ2)
	$(CC) $(CFLAGS) -o $(PROG2) $(OBJ2) $(LIBS)
$(PROG3):$(OBJ3)
	$(CC) $(CFLAGS) -o $(PROG3) $(OBJ3) $(LIBS)
.cpp.o:
	$(CC) $(CFLAGS) -c $?

clean:
	rm -f $(all) $(OBJ) $(OBJ2) $(OBJ3)
