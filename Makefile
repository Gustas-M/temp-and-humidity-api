CC = gcc
CFLAGS = -Wall -Wextra -fPIC -I
LDFLAGS = -shared
TARGET = libbmp280.so
SRC = libbmp280.c
INC = libbmp280.h

LIB_PREFIX = /lib
INC_PREFIX = /usr/include

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install: $(TARGET)
	cp $(INC) $(INC_PREFIX)/
	cp $(TARGET) $(LIB_PREFIX)/

clean:
	rm -f $(LIB_PREFIX)/$(TARGET)
	rm -f $(INC_PREFIX)/$(INC)
