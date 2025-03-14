CC = gcc 
CFLAGS = -Wall -Wextra -std=c11 
LIBRARY = ncurses
TARGET = gato
SRC = gato.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -l$(LIBRARY)


run: all
	./$(TARGET)
	
clean: 
	rm -f $(TARGET)
