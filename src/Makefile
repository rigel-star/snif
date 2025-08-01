../lib:
	mkdir -p ../lib

libsnif.so: eth_parser.c sniffer.c library.c
	gcc -shared -fPIC -Wall -Wextra -pedantic -ggdb -O2 $^ -lpcap -o ../lib/$@

main: sniffer.c eth_parser.c
	gcc $^ -lpcap -o main

lite: sniffer.c eth_parser.c
	gcc -shared -fPIC -Wall -Wextra -pedantic -ggdb -O0 $^ -lpcap -o ../lib/libsnif.so

.PHONY: clean
clean:
	rm -f *.o
	rm -f main