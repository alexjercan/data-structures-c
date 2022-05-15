CC=gcc
CFLAGS=-Wall -g -O2
LDFLAGS=
LDLIBS=-lm

HDR_DIR := include
OBJ_DIR := obj
SRC_DIR := src

HDRS=$(wildcard $(HDR_DIR)/*.h)
OBJS=$(patsubst $(HDR_DIR)/%.h,$(OBJ_DIR)/%.o,$(HDRS))
SRCS=$(patsubst $(HDR_DIR)/%.h,$(SRC_DIR)/%.c,$(HDRS))

main: main.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDR_DIR)/%.h
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR) main.o main
