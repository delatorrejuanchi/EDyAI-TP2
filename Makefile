main: main.c util.o slist.o spila.o cdlist.o cdcola.o tree.o
			gcc -g -o main main.c util.o slist.o spila.o cdlist.o cdcola.o tree.o -Wall -pedantic

util.o: util.c
				gcc -g -c util.c -Wall -pedantic

slist.o: slist/slist.c
				 gcc -g -c slist/slist.c -Wall -pedantic

spila.o: spila/spila.c
				 gcc -g -c spila/spila.c -Wall -pedantic

cdlist.o: cdlist/cdlist.c
				 gcc -g -c cdlist/cdlist.c -Wall -pedantic

cdcola.o: cdcola/cdcola.c
				 gcc -g -c cdcola/cdcola.c -Wall -pedantic

tree.o: tree/tree.c
				gcc -g -c tree/tree.c -Wall -pedantic
