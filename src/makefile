main: *.c *.h
	gcc *.c -Wall -Wextra -pedantic -O3 -o main

debug: *.c *.h
	gcc *.c -Wall -Wextra -pedantic -g -o debug

run: main
	./main

run-debug: debug
	gdb ./debug

lib: string.c sais.c string.h sais.h
	gcc -c string.c sais.c -Wall -Wextra -pedantic -O3
	ar rcs libstring.a *.o
	rm -f *.o

clean:
	rm -f main debug libstring.a