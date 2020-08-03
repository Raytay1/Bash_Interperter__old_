cmdparse.o: cmdparse.c cmdparse.h
	gcc -g -c cmdparse.c -o cmdparse.o

main.o: main.c cmdparse.h
	gcc -g -c main.c -o main.o

main: main.o cmdparse.o
	gcc -g -o myshell main.o cmdparse.o

clean:
	rm -f *.o
