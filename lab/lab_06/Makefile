CC := gcc
FLAGS := -g -std=c99 -Wall -Werror -pedantic

ifeq ($(mode), debug)
	FLAGS += -g6
endif

ifeq ($(mode), release)
	FLAGS += -DNDEBUG -g0
endif

app.exe : main.o tree.o hash_table.o
	$(CC) -o app.exe main.o tree.o hash_table.o

main.o : main.c hash_table.h tree.h define.h
	$(CC) $(FLAGS) -c main.c

tree.o : tree.c hash_table.h tree.h define.h
	$(CC) $(FLAGS) -c tree.c

htable.o : htable.c hash_table.h tree.h define.h
	$(CC) $(FLAGS) -c htable.c

st : main.exe
	./main.exe

clean :
	rm *.o *.exe
