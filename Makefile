PROJ=scanner
CFLAGS=-std=c11
CC=gcc
RM=rm -f

$(PROJ) : $(PROJ).c
		$(CC) $(CFLAGS) -o $(PROJ) $(PROJ).c

clean :
	$(RM) *.o $(PROJ)

