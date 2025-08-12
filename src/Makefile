EXT_LIBS = -lpcap

CC = gcc
BASE_FLAGS = -fPIC -Wall -Wextra -pedantic -ggdb -O3
CC_FLAGS = -shared $(BASE_FLAGS)
O_FLAGS = $(BASE_FLAGS)

SRCS = $(wildcard *.c vendor/cJSON/*.c ip/*.c arp/*.c)
OBJS = $(patsubst %.c, build/%.o, $(SRCS)) # ---> patsubst(pattern, replacement, text) = pattern substitute

TARGET = ../lib/libsnif.so

all: $(TARGET)

$(TARGET): $(OBJS) | ../lib
	$(CC) $(CC_FLAGS) $(OBJS) $(EXT_LIBS) -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(O_FLAGS) $< -o $@

../lib:
	mkdir -p ../lib

.PHONY: clean
clean:
	rm -rf *.o main ../lib/$(notdir $(TARGET)) ./build