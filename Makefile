main: main.c slist.o tablahash.o
			gcc -o main main.c slist.o tablahash.o -Wall -pedantic

slist.o: slist.c
				 gcc -c slist.c -Wall -pedantic

tablahash.o: tablahash.c
						 gcc -c tablahash.c -Wall -pedantic
