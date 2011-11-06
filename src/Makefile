MF=	Makefile

CC=cc
CFLAGS+=	-g -I$(PROJ_INCLUDE) -DDEBUG
LFLAGS= -lm

EXE=puma

SRC= \
	main.c \
	io.c \
	pde.c \
	puma.c

TEST_SRC=\
	$(PROJ_TEST)/test.c \
	$(SRC)

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)
TEST_OBJ=$(TEST_SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE) test

test: $(TEST_SRC) $(OBJ)
	$(CC) $(CFLAGS) -c $(PROJ_TEST)/test.c
	$(CC) $(FFLAGS) -o $@ $(TEST_OBJ) $(LFLAGS)

$(EXE):	$(OBJ)
	$(CC) $(FFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ) $(TEST_OBJ):	$(MF) $(INC)

all:
	cp -f $(EXE) $(PROJ_BIN)

clean:
	rm -f $(OBJ) $(EXE) $(TEST_OBJ) core
