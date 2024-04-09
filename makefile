OBJS	= mysh.o
SOURCE	= mysh.c
HEADER	= 
OUT	= mysh
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

spchk.o: spchk.c
	$(CC) $(FLAGS) mysh.c 


clean:
	rm -f $(OBJS) $(OUT)
