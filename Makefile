CC = gcc
CFLAGS = -Wall -Wextra -O2 -MMD -MP
LDLIBS = -lncursesw -lm

TARGET = monitor
SRCS = display.c textCONST.c main.c memINFO.c cpuINFO.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) $(SRCS:.c=.d)

.PHONY: all clean
