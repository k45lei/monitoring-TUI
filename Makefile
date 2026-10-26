CC = gcc
CFLAGS = -Wall -Wextra -O2
LDLIBS = -lncursesw

TARGET = display
SRCS = display.c textCONST.c main.c memINFO.c cpuINFO.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
