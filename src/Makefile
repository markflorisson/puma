#CC=pgcc
CC=gcc
CFLAGS=	-g -I$(INCLUDE_DIR) -DDEBUG
LFLAGS= -lm

INCLUDE_DIR=../include
BIN_DIR=../bin

EXE=$(BIN_DIR)/puma

SRC= \
    puma.c \
	io.c \
	main.c

.SUFFIXES:
.SUFFIXES: .c .o

# OBJ=$(SRC:.c=.o)
OBJ=$(addprefix $(BIN_DIR)/, $(SRC:.c=.o))

MF=Makefile

.c.o:
	$(CC) $(CFLAGS) -c $< -o $(BIN_DIR)/$<

all:    $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ): $(SRC)
$(SRC): $(MF) $(INC)
	$(CC) $(CFLAGS) -c $@ -o $(BIN_DIR)/$(@:.c=.o)

#$(SRC): $(MF)
#	$(CC) $(CFLAGS) -c $< -o $(BIN_DIR)/$<


clean:
	rm -f $(OBJ) $(EXE) core
