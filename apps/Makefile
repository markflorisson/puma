MF=	Makefile

CC=pgcc
CFLAGS=	-g -I$(PROJ_HOME)/utils -DDEBUG
LFLAGS= -lm

EXE=puma

SRC= \
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
