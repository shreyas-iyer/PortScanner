CC = g++
TARGET = scanner

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)