CC=c++
LINKER=
FLAGS=-Wall -Wextra -O3
OBJ=obj/main.o obj/statistik.o
MKDIR=mkdir -p

all: dirs statistik.out

dirs:
	$(MKDIR) obj bin

statistik.out: $(OBJ)
	$(CC) $(FLAGS) -o bin/statistik.out $(OBJ) $(LINKER)

obj/main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o obj/main.o $(LINKER)

obj/statistik.o: statistik.cpp
	$(CC) $(FLAGS) -c statistik.cpp -o obj/statistik.o $(LINKER)

clean:
	rm obj/main.o obj/statistik.o
