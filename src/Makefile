libsnif.so: main.c
	gcc -shared -fPIC -Wall -Wextra -pedantic -ggdb -O2 $^ -o ../lib/$@

.PHONY: clean
clean:
	rm -f *.o
	rm -f main
