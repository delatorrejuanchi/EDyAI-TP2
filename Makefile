main: main.c slist.o tree.o
			gcc -o main main.c slist.o tree.o -Wall -pedantic

slist.o: slist.c
				 gcc -c slist.c -Wall -pedantic

tablahash.o: tablahash.c
						 gcc -c tablahash.c -Wall -pedantic

tree.o: tree.c
				gcc -c tree.c -Wall -pedantic
