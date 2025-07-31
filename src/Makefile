../lib:
	mkdir -p ../lib

libsnif.so: library.c sniffer.c
	gcc -shared -fPIC -Wall -Wextra -pedantic -ggdb -O2 $^ -lpcap -o ../lib/$@

.PHONY: clean
clean:
	rm -f *.o
	rm -f main