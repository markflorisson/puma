MF=	Makefile

#CC=pgcc
CC=cc
CFLAGS=	-g -I$(PROJ_HOME)/include -DDEBUG
LFLAGS= -lm

EXE=puma

SRC= \
	main.c \
	io.c \
	puma.c

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(EXE):	$(OBJ)
	$(CC) $(FFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF) $(INC)

clean:
	rm -f $(OBJ) $(EXE) core
