CC              = gcc
SRC             = curparse.c
TARGET          = resparse
OBJ             = $(SRC:.c=.o)
DDDEBUG         =
#------------------------------------------------------

$(TARGET): $(OBJ)
	$(CC)  -o $@ $(OBJ) 

$(OBJ): $(SRC)
	$(CC) -g  -c $(SRC) $(DDDEBUG)

clean:
	rm -f *.o *~ $(TARGET)





