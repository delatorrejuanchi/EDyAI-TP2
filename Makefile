main: main.c util.o slist.o spila.o cdlist.o cdcola.o tree.o
			gcc -o main main.c util.o slist.o spila.o cdlist.o cdcola.o tree.o -Wall -pedantic

util.o: util.c
				gcc -c util.c -Wall -pedantic

slist.o: slist/slist.c
				 gcc -c slist/slist.c -Wall -pedantic

spila.o: spila/spila.c
				 gcc -c spila/spila.c -Wall -pedantic

cdlist.o: cdlist/cdlist.c
				 gcc -c cdlist/cdlist.c -Wall -pedantic

cdcola.o: cdcola/cdcola.c
				 gcc -c cdcola/cdcola.c -Wall -pedantic

tree.o: tree/tree.c
				gcc -c tree/tree.c -Wall -pedantic
