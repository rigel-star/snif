../lib:
	mkdir -p ../lib

CC_FLAGS=-shared -fPIC -Wall -Wextra -pedantic -ggdb -O2
SRCS = $(wildcard *.c vendor/cJSON/*.c ip/*.c)
TARGET=libsnif.so

$(TARGET): $(SRCS)
	gcc $(CC_FLAGS) $^ -lpcap -o ../lib/$@

.PHONY: clean
clean:
	rm -f *.o
	rm -f main
	rm -f libsnif.so