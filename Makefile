CC = g++
CFLAGS = -fopenmp
TARGET = main
SOURCE = functions.cpp main.cpp tictactoe.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) 

clean:
	rm -f $(TARGET)