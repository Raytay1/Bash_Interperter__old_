# Makefile
# ***************************************************************

PROGRAMS = \
	main \
	clean \

# ***************************************************************
# Parameters to control Makefile operation
# Note that the gccx command script must be defined


CC = gcc
CFLAGS =

# ***************************************************************
# Entry to bring the package up to date

all: $(PROGRAMS)

# ***************************************************************
# Standard entries to remove files from the directories
#    tidy    -- eliminate unwanted files
#    scratch -- delete derived files in preparation for rebuild

tidy:
	rm -f ,* .,* *~ core a.out graphics.ps

scratch: tidy
	rm -f *.o *.a $(PROGRAMS)

# ***************************************************************
# C compilations

cmdparse.o: cmdparse.c cmdparse.h
	$(CC) -g -c cmdparse.c -o cmdparse.o

main.o: main.c cmdparse.h
	$(CC) -g -c main.c -o main.o

# ***************************************************************
# Executable programs

main: main.o cmdparse.o
	$(CC) -g -o myshell main.o cmdparse.o

clean:
	rm -f *.o
